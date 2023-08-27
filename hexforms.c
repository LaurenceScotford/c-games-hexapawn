/**
 * @file hexforms.c
 * @brief Functions for managing the forms in Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"

static apr_pool_t * hex_forms_pool;

static hexform_t hexforms[] = {
	{FORM_PLAYER_MOVE, NULL, WIN_MOVE_FORM, NULL, MOVE_FORM_Y, MOVE_FORM_X, MOVE_FORM_HEIGHT, MOVE_FORM_WIDTH, MOVE_FORM_FIELDS, NULL,
		(form_field_t[MOVE_FORM_FIELDS]){
		 	{ MOVE_FORM_HEIGHT, MOVE_FORM_FIELD_WIDTH, MOVE_FORM_FIELD_Y, MOVE_FORM_FROM_FIELD_X, 0, 0, STR_MOVE_FORM_FROM, MOVE_FORM_FIELD_Y, MOVE_FORM_FROM_LABEL_X },
			{ MOVE_FORM_HEIGHT, MOVE_FORM_FIELD_WIDTH, MOVE_FORM_FIELD_Y, MOVE_FORM_TO_FIELD_X, 0, 0, STR_MOVE_FORM_TO, MOVE_FORM_FIELD_Y, MOVE_FORM_TO_LABEL_X }
		}
	}
};

/**
 * @brief Initialise forms
 */
void initialise_forms() {
    // Create a data pool for hexforms
    if (apr_pool_create(&hex_forms_pool, NULL) != APR_SUCCESS) {
		exit_with_error(STR_DATA_ERROR);
	}

	for (int i = 0; i < ARRAY_SIZE(hexforms); i++) {
		// Create the form fields
		hexforms[i].fields = apr_palloc(hex_forms_pool, sizeof(FIELD *) * hexforms[i].num_fields + 1);
	
		if (hexforms[i].fields == NULL) {
			exit_with_error(STR_FORM_ERROR);
		}
	
		int rows, cols; 
		WINDOW * window = get_hexwindow(hexforms[i].window)->w_ptr;
		
		for	(int j = 0; j < hexforms[i].num_fields; j++) {
			form_field_t field_desc = hexforms[i].field_list[j];
			hexforms[i].fields[j] = new_field(field_desc.height, field_desc.width, field_desc.y, field_desc.x, field_desc.vis_rows, field_desc.buffers);

			if (hexforms[i].fields[j] == NULL) {
				exit_with_error(STR_FORM_ERROR);
			}

			set_field_opts(hexforms[i].fields[j], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE | O_STATIC | O_BLANK);
			set_field_back(hexforms[i].fields[j], A_UNDERLINE);
		}

		hexforms[i].fields[hexforms[i].num_fields] = NULL;
	
		// Create the form
		hexforms[i].form = new_form(hexforms[i].fields);

		if (hexforms[i].form == NULL) {
			exit_with_error(STR_FORM_ERROR);
		}

		// Set the form's window
		set_form_win(hexforms[i].form, window);
		scale_form(hexforms[i].form, &rows, &cols);
		hexforms[i].subwindow = derwin(window, rows, cols, 0, 0);
	
		if (hexforms[i].subwindow == NULL) {
			exit_with_error(STR_FORM_ERROR);
		}

		set_form_sub(hexforms[i].form, hexforms[i].subwindow);
		int result = post_form(hexforms[i].form);
		
		if (result != E_OK) {
			exit_with_error(STR_FORM_ERROR);
		}

		// Draw the labels on the parent form
		for	(int j = 0; j < hexforms[i].num_fields; j++) {
			form_field_t field_desc = hexforms[i].field_list[j];
			mvwprintw(window, field_desc.label_y, field_desc.label_x, field_desc.label);
		}

		pos_form_cursor(hexforms[i].form);
		wrefresh(window);
	}
}

/**
 * @brief Return a pointer to the struct that matches the form uid or NULL if no match is found
 * @param f_id The unique ID of the required form
 * @returns A pointer to the structure describing the form or NULL if a match cannot be found
 */
hexform_t * get_hexform(form_id_t f_id) {
	for (int i = 0; i < ARRAY_SIZE(hexforms); i++) {
		if (hexforms[i].uid == f_id) {
			return &hexforms[i];
		}
	}

	return NULL;
}

/**
 * @brief Safely destroy the game forms
 */
void destroy_forms() {
	for (int i = 0; i < ARRAY_SIZE(hexforms); i++) {
		unpost_form(hexforms[i].form);
		free_form(hexforms[i].form);
		
		for (int j = 0; j < hexforms[i].num_fields; j++) {
			free_field(hexforms[i].fields[j]);	
		}
	}

    apr_pool_destroy(hex_forms_pool);
}

/**
 * @brief Handle navigation for the given form
 * @param f_id The unique ID of the form to be navigated
 * @returns A nav_ouput struct indicating that navigation should continue
 */
nav_output_t form_navigation(form_id_t f_id, int c) {
	hexform_t *form = get_hexform(f_id);
	FORM *f_ptr = form->form;
	form_driver(f_ptr, REQ_OVL_MODE);

	switch(c) {
		case '\t':
			form_driver(f_ptr, REQ_NEXT_FIELD);
			break;
		case KEY_BTAB:
			form_driver(f_ptr, REQ_PREV_FIELD);
		default:
			form_driver(f_ptr, c);
	}

	show_window(form->window);

	return (nav_output_t){true, -1};
}

/**
 * @brief Return a pointer to the current value for the given field in given form
 * @param f_id The unique ID of the form to be queried
 * @param field_num	The zero-indexed number of the field to query
 * @returns A pointer to the field buffer
 */
char * get_form_field_pointer(form_id_t f_id, int field_num) {
	hexform_t *form = get_hexform(f_id);
	form_driver(form->form, REQ_VALIDATION); // Ensure current value is written to buffer
	return field_buffer(form->fields[field_num], 0);
}

/**
 * @brief Resets all the fields in a form
 * @param f_id The unique ID of the form to be reset
 * @param alert	True if an audio alert is to accompany the reset
 */
void reset_form(form_id_t f_id, bool alert) {
	if (alert) {
		beep();
	}

	hexform_t *form = get_hexform(f_id);
	form_driver(form->form, REQ_FIRST_FIELD);

	for (int i = 0; i < ARRAY_SIZE(form->fields) - 1; i++) {
		form_driver(form->form, REQ_CLR_FIELD);
		form_driver(form->form, REQ_NEXT_FIELD);
	}
}