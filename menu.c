#include "menu.h"

#include <string.h>
#include <unistd.h>

void draw_game_setting(menu_t * menu, char key);
void draw_player_count(menu_t * menu, char key);
void draw_button(int x, int y, bool active, char* text) {
    if (active) {
        set_background_color(ACTIVE_BUTTON);
    } else {
        set_background_color(BUTTON_COLOR);
    }
    draw_rect_full(x,y, BUTTON_WIDTH,BUTTON_HEIGHT);
    move_to(((2*CONSOLE_WIDTH)-(int)strlen(text))/2 + 1 ,y + 2);
    print_text(text);
    reset_text_effect();
}

void draw_controls() {
    set_background_color(8);
    draw_rect(2,(CONSOLE_HEIGHT/2)-1,3,3);
    move_to(7,(CONSOLE_HEIGHT/2)+1);
    print_text("A ");
    draw_rect(6,(CONSOLE_HEIGHT/2)-5,3,3);
    move_to(15,(CONSOLE_HEIGHT/2)-3);
    print_text("W ");
    draw_rect(6,(CONSOLE_HEIGHT/2)-1,3,3);
    move_to(15,(CONSOLE_HEIGHT/2)+1);
    print_text("S ");
    draw_rect(10,(CONSOLE_HEIGHT/2)-1,3,3);
    move_to(23,(CONSOLE_HEIGHT/2)+1);
    print_text("D ");

    draw_rect(CONSOLE_WIDTH - 12,(CONSOLE_HEIGHT/2)-5,9,3);
    move_to((CONSOLE_WIDTH*2)-21,(CONSOLE_HEIGHT/2)-3);
    print_text("    Enter     ");
    draw_rect(CONSOLE_WIDTH - 12,(CONSOLE_HEIGHT/2)-1,9,3);
    move_to((CONSOLE_WIDTH*2)-21,(CONSOLE_HEIGHT/2)+1);
    print_text("   ESCape/q   ");
}

void draw_border() {
    set_background_color(6);
    draw_rect(15,0,30,CONSOLE_HEIGHT);
    set_background_color(4);
    draw_rect(0,0,15,CONSOLE_HEIGHT);
    draw_rect(CONSOLE_WIDTH-15,0,15,CONSOLE_HEIGHT);
    set_background_color(2);
    draw_rect(2,2, 11,5);
    set_background_color(3);
    draw_rect(2,CONSOLE_HEIGHT - 7, 11,5);
    set_background_color(5);
    draw_rect(CONSOLE_WIDTH - 13 ,2, 11,5);
    set_background_color(7);
    draw_rect(CONSOLE_WIDTH - 13,CONSOLE_HEIGHT - 7, 11,5);
}

void initialize_menu(menu_t *menu) {
    for (int i = 0; i <BUTTON_COUNT; i++) {
        menu->buttons->isActive = false;
    }
    menu->gameSize = BIG;
    menu->gameType = LOOP;
    menu->menuType = 1;
    menu->time = 0;
    menu->playerCount = 1;
}

void draw_menu() {
    draw_border();
    draw_controls();
}

void reset_buttons(menu_t * menu) {
    reset_text_effect();
    draw_rect_full((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-2*BUTTON_HEIGHT-2,BUTTON_WIDTH, BUTTON_HEIGHT*5);
    menu->buttons[0].isActive =true;
    menu->buttons[1].isActive = false;
    menu->buttons[2].isActive = false;
    menu->buttons[3].isActive = false;
}

void button_scroll(menu_t * menu, char key, int buttonCount) {
    if (buttonCount > BUTTON_COUNT) {
        buttonCount = BUTTON_COUNT;
    } else if (buttonCount < 2) {
        return;
    }
    if (key == 'w') {
        for (int i = 0; i < buttonCount; i++) {
            if (menu->buttons[i].isActive) {
                int current = i;
                menu->buttons[current].isActive = false;
                current--;
                if (current < 0) {
                    current = buttonCount -1;
                }
                menu->buttons[current].isActive = true;
                break;
            }
        }
    }
    if (key == 's') {
        for (int i = buttonCount - 1; i >=0; i--) {
            if (menu->buttons[i].isActive) {
                int current = i;
                menu->buttons[current].isActive = false;
                current++;
                if (current >= buttonCount) {
                    current = 0;
                }
                menu->buttons[current].isActive = true;
                break;
            }
        }
    }
}

void draw_main_menu(menu_t* menu,char key) {
    button_scroll(menu,key,3);
    if (key == '\r' || key == 'e') {
        if (menu->buttons[0].isActive) {
            menu->menuType = 5;
            reset_text_effect();
            reset_buttons(menu);
            draw_player_count(menu,' ');
            return;
        }
        if (menu->buttons[1].isActive) {
            menu->menuType = 3;
            reset_buttons(menu);
            return;
        }
        if (menu->buttons[2].isActive) {
            menu->menuType = -1;
        }
    }
    if (key == 27 || key == 'q') {
        menu->menuType = -1;
    }
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,menu->buttons[0].isActive,"New game");
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2),menu->buttons[1].isActive,"Join");
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)+BUTTON_HEIGHT + 1,menu->buttons[2].isActive,"Exit");
}

void draw_game_setting(menu_t * menu, char key) {
    button_scroll(menu,key,4);
    if (key == '\r' || key == 'e') {
        if (menu->buttons[0].isActive) {
            menu->gameSize = (menu->gameSize+1)%3;
        }
        if (menu->buttons[1].isActive) {
            menu->gameType = (menu->gameType+1)%2;
        }
        if (menu->buttons[2].isActive) {
            menu->time = (menu->time +30)%150;
        }
        if (menu->buttons[3].isActive) {
            reset_text_effect();
            reset_buttons(menu);
            menu->menuType = 7;
            return;
        }
    }
    if (key == 27 ||key == 'q') {
        menu->menuType = 1;
        reset_buttons(menu);
        draw_main_menu(menu,' ');
        return;
    }
    char * size = "< 28X28 >";
    switch (menu->gameSize) {
        case SMALL:
            size = "< 10X10 >";
            break;
        case MEDIUM:
            size = "< 19X19 >";
            break;
        case BIG:
            size = "< 28X28 >";
            break;
    }
    char * game = "< Loop >";
    switch (menu->gameType) {
        case LOOP:
            game = "< Loop >";
            break;
        case EDGE:
            game = "< Edge >";
            break;
    }
    char time[10];
    if (menu->time == 0) {
        strcpy(time,"< inf >");
    } else {
        sprintf(time,"< %d s >",menu->time);
    }
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-2*BUTTON_HEIGHT-2,menu->buttons[0].isActive,size);
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,menu->buttons[1].isActive,game);
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2),menu->buttons[2].isActive,time);
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)+BUTTON_HEIGHT + 1,menu->buttons[3].isActive,"Continue");
    move_to(5*2+1, 19);
    print_text(size);
    move_to(5*2+1, 20);
    print_text(game);
    move_to(5*2+1, 21);
    print_text(time);

}

void draw_pause_menu(menu_t * menu, char key) {
    if (key == 'w' || key == 's') {
        if (menu->buttons[0].isActive) {
            menu->buttons[0].isActive = false;
            menu->buttons[1].isActive = true;
        } else {
            menu->buttons[0].isActive = true;
            menu->buttons[1].isActive = false;
        }
    }
    if (key == '\r' || key == 'e') {
        if (menu->buttons[0].isActive) {
            menu->menuType = 0;
            reset_text_effect();
            draw_rect_full((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,BUTTON_WIDTH, BUTTON_HEIGHT*3);
            menu->buttons[0].isActive =true;
            menu->buttons[1].isActive = false;
            menu->buttons[2].isActive = false;
            menu->buttons[3].isActive = false;
            sleep(3);
            return;
        }
        if (menu->buttons[1].isActive) {
            menu->menuType = 1;
            reset_buttons(menu);
            draw_main_menu(menu,' ');
            return;
        }
    }
    if (key == 27 || key == 'q') {
        menu->menuType = 1;
        reset_buttons(menu);
        draw_main_menu(menu,' ');
        return;
    }
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,menu->buttons[0].isActive,"Continue");
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)+1,menu->buttons[1].isActive,"Leave");
}

void join_server(menu_t * menu, char key) {
    menu->menuType = 0;
    sleep(3);
}

void draw_player_count(menu_t * menu, char key) {
    if (key == 'w' || key == 's') {
        if (menu->buttons[0].isActive) {
            menu->buttons[0].isActive = false;
            menu->buttons[1].isActive = true;
        } else {
            menu->buttons[0].isActive = true;
            menu->buttons[1].isActive = false;
        }
    }
    if (key == '\r' || key == 'e') {
        if (menu->buttons[0].isActive) {
            menu->menuType = 2;
            menu->playerCount = 1;
        }
        if (menu->buttons[1].isActive) {
            menu->menuType = 2;
            menu->playerCount = 4;
        }
        reset_buttons(menu);
        draw_game_setting(menu,' ');
        return;
    }
    if (key == 27 || key == 'q') {
        menu->menuType = 1;
        reset_buttons(menu);
        draw_main_menu(menu,' ');
        return;
    }
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,menu->buttons[0].isActive,"Singleplayer");
    draw_button((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)+1,menu->buttons[1].isActive,"Multiplayer");
}

void disconnect(menu_t * menu, char key) {
    menu->menuType = 1;
    draw_main_menu(menu,' ');
}
void create_and_join_server(menu_t * menu, char key) {
    menu->menuType = 0;
    sleep(3);
}

int update_menu(menu_t * menu,char key) {
    switch (menu->menuType) {
        case 0 :
            if (key == 'q' || key == 27) {
                menu->menuType = 4;
                draw_pause_menu(menu,' ');
            }
            break;
        case 1:
            draw_main_menu(menu,key);
            break;
        case 2:
            draw_game_setting(menu,key);
            break;
        case 3:
            join_server(menu,key);
            break;
        case 4:
            draw_pause_menu(menu,key);
            break;
        case 5:
            draw_player_count(menu,key);
            break;
        case 6:
            disconnect(menu, key);
            break;
        case 7:
            create_and_join_server(menu,key);
            break;
        default:
            return -1;
    }
    return menu->menuType;
}


void create_buttons(menu_t * menu) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        menu->buttons[i].isActive = false;
    }
    menu->buttons[0].isActive = true;
    menu->menuType = 1;
    menu->gameSize = BIG;
    menu->gameType = LOOP;
    menu->time = 0;
}
