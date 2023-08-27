/**
 * @file hexforms.h
 * @brief Functions for managing the forms in Hexapawn
 * @author Laurence Scotford
 */

#ifndef __Hexapawn__hexforms__
#define __Hexapawn__hexforms__

void initialise_forms(void);
hexform_t * get_hexform(form_id_t f_id);
void destroy_forms(void);
nav_output_t form_navigation(form_id_t f_id, int c);
char *get_form_field_pointer(form_id_t f_id, int field_num);
void reset_form(form_id_t f_id, bool alert);

#endif /* defined(__Hexapawn_hexforms__) */