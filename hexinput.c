/**
 * @file hexinput.c
 * @brief Functions for managing user input in Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"

/**
 * @brief Handle navigation within a screen for a given menu and/or a given item
 * @param m_id The unique ID of the menu to be navigated (MENU_NONE if none)
 * @param f_id The unique ID of the form to be navigated (FORM_NONE if none)
 * @returns An output code when navigation ends
 */
int navigation_dispatch(menu_id_t m_id, form_id_t f_id) {
    hexmenu_t *m_ptr = NULL;    // Store pointer to menu
    hexform_t *f_ptr = NULL;    // Store pointer to form
    hexwindow_t *ws_ptr = NULL;        // Store pointer to common window
    nav_output_t nav_output;    // Stores the output from each nav operation
    int c;                      // Used to collect input characters

    // Get pointers to the relevant menu and / or form
    if (m_id != MENU_NONE) {
        m_ptr = get_hexmenu(m_id);
    }

    if (f_id != FORM_NONE) {
        f_ptr = get_hexform(f_id);
    }

    // Check the combination of items is valid
    if (!(m_ptr || f_ptr)) {
        exit_with_error(STR_INVALID_NAV_DISPATCH);
    }

    // If there is both a form and a menu, get lowest common window
    if (m_ptr && f_ptr) {
        window_id_t commonWindow = get_lowest_common_window(m_ptr->window, f_ptr->window);
    
        // Get the pointer to this window, if it exists
        if (commonWindow != WIN_NONE) {
            ws_ptr = get_hexwindow(commonWindow);
        } else {
            exit_with_error(STR_INVALID_NAV_DISPATCH);
        }
    } else {
        // Otherwise set the window pointer to the relevant window for the single item
        ws_ptr = get_hexwindow(m_ptr ? m_ptr->window : f_ptr->window);
    }

    nav_output = (nav_output_t){ true, 0 };

    // Continue dispatching navigation events until the end of nav is signalled
    while (nav_output.in_nav) {
        c = wgetch(ws_ptr->w_ptr);

        switch(c) {
            case KEY_DOWN:
            case KEY_UP:
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_HOME:
            case KEY_END:
            case '\n':
                // dispatch to menu if one is set
                if (m_ptr)
                {
                    nav_output = menu_navigation(m_id, c);
                }
                break;
            default:
                // dispatch to form if one is set
                if (f_ptr) {
                    nav_output = form_navigation(f_id, c);
                }
        }
    }

    return nav_output.output;
}