#include "menu.h"

#include <string.h>
#include <unistd.h>

void DrawGameSetting(menu_t * menu, char key);
void DrawPlayerCount(menu_t * menu, char key);
void DrawButton(int x, int y, bool active, char* text) {
    if (active) {
        SetBackgroundColor(ACTIVE_BUTTON);
    } else {
        SetBackgroundColor(BUTTON_COLOR);
    }
    DrawRectFull(x,y, BUTTON_WIDTH,BUTTON_HEIGHT);
    MoveTo(((2*CONSOLE_WIDTH)-(int)strlen(text))/2 + 1 ,y + 2);
    Write(text);
    ResetTextEffect();
}

void DrawControls() {
    SetBackgroundColor(8);
    DrawRect(2,(CONSOLE_HEIGHT/2)-1,3,3);
    MoveTo(7,(CONSOLE_HEIGHT/2)+1);
    Write("A ");
    DrawRect(6,(CONSOLE_HEIGHT/2)-5,3,3);
    MoveTo(15,(CONSOLE_HEIGHT/2)-3);
    Write("W ");
    DrawRect(6,(CONSOLE_HEIGHT/2)-1,3,3);
    MoveTo(15,(CONSOLE_HEIGHT/2)+1);
    Write("S ");
    DrawRect(10,(CONSOLE_HEIGHT/2)-1,3,3);
    MoveTo(23,(CONSOLE_HEIGHT/2)+1);
    Write("D ");

    DrawRect(CONSOLE_WIDTH - 12,(CONSOLE_HEIGHT/2)-5,9,3);
    MoveTo((CONSOLE_WIDTH*2)-21,(CONSOLE_HEIGHT/2)-3);
    Write("    Enter     ");
    DrawRect(CONSOLE_WIDTH - 12,(CONSOLE_HEIGHT/2)-1,9,3);
    MoveTo((CONSOLE_WIDTH*2)-21,(CONSOLE_HEIGHT/2)+1);
    Write("   ESCape/q   ");
}

void DrawBorder() {
    SetBackgroundColor(6);
    DrawRect(15,0,30,CONSOLE_HEIGHT);
    SetBackgroundColor(4);
    DrawRect(0,0,15,CONSOLE_HEIGHT);
    DrawRect(CONSOLE_WIDTH-15,0,15,CONSOLE_HEIGHT);
    SetBackgroundColor(2);
    DrawRect(2,2, 11,5);
    SetBackgroundColor(3);
    DrawRect(2,CONSOLE_HEIGHT - 7, 11,5);
    SetBackgroundColor(5);
    DrawRect(CONSOLE_WIDTH - 13 ,2, 11,5);
    SetBackgroundColor(7);
    DrawRect(CONSOLE_WIDTH - 13,CONSOLE_HEIGHT - 7, 11,5);
}

void DrawMenu() {
    DrawBorder();
    DrawControls();
}

void ResetButtons(menu_t * menu) {
    ResetTextEffect();
    DrawRectFull((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-2*BUTTON_HEIGHT-2,BUTTON_WIDTH, BUTTON_HEIGHT*5);
    menu->buttons[0].isActive =true;
    menu->buttons[1].isActive = false;
    menu->buttons[2].isActive = false;
    menu->buttons[3].isActive = false;
}

void ButtonScroll(menu_t * menu, char key, int buttonCount) {
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

void DrawMainMenu(menu_t* menu,char key) {
    ButtonScroll(menu,key,3);
    if (key == '\r' || key == 'e') {
        if (menu->buttons[0].isActive) {
            menu->menuType = 5;
            ResetTextEffect();
            ResetButtons(menu);
            DrawPlayerCount(menu,' ');
            return;
        }
        if (menu->buttons[1].isActive) {
            menu->menuType = 3;
            ResetButtons(menu);
            return;
        }
        if (menu->buttons[2].isActive) {
            menu->menuType = -1;
        }
    }
    if (key == 27 || key == 'q') {
        menu->menuType = -1;
    }
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,menu->buttons[0].isActive,"New game");
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2),menu->buttons[1].isActive,"Join");
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)+BUTTON_HEIGHT + 1,menu->buttons[2].isActive,"Exit");
}

void DrawGameSetting(menu_t * menu, char key) {
    ButtonScroll(menu,key,4);
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
            ResetTextEffect();
            ResetButtons(menu);
            menu->menuType = 7;
            return;
        }
    }
    if (key == 27 ||key == 'q') {
        menu->menuType = 1;
        ResetButtons(menu);
        DrawMainMenu(menu,' ');
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
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-2*BUTTON_HEIGHT-2,menu->buttons[0].isActive,size);
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,menu->buttons[1].isActive,game);
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2),menu->buttons[2].isActive,time);
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)+BUTTON_HEIGHT + 1,menu->buttons[3].isActive,"Continue");
    MoveTo(5*2+1, 19);
    Write(size);
    MoveTo(5*2+1, 20);
    Write(game);
    MoveTo(5*2+1, 21);
    Write(time);

}

void DrawPauseMenu(menu_t * menu, char key) {
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
            ResetButtons(menu);
            sleep(3);
            return;
        }
        if (menu->buttons[1].isActive) {
            menu->menuType = 1;
            ResetButtons(menu);
            DrawMainMenu(menu,' ');
            return;
        }
    }
    if (key == 27 || key == 'q') {
        menu->menuType = 1;
        ResetButtons(menu);
        DrawMainMenu(menu,' ');
        return;
    }
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,menu->buttons[0].isActive,"Continue");
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)+1,menu->buttons[1].isActive,"Leave");
}

void JoinServer(menu_t * menu, char key) {
    menu->menuType = 0;
    sleep(3);
}

void DrawPlayerCount(menu_t * menu, char key) {
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
        ResetButtons(menu);
        DrawGameSetting(menu,' ');
        return;
    }
    if (key == 27 || key == 'q') {
        menu->menuType = 1;
        ResetButtons(menu);
        DrawMainMenu(menu,' ');
        return;
    }
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)-BUTTON_HEIGHT-1,menu->buttons[0].isActive,"Singleplayer");
    DrawButton((CONSOLE_WIDTH-BUTTON_WIDTH)/2,(CONSOLE_HEIGHT/2)+1,menu->buttons[1].isActive,"Multiplayer");
}

void Disconnect(menu_t * menu, char key) {
    menu->menuType = 1;
    DrawMainMenu(menu,' ');
}
void CreateAndJoinServer(menu_t * menu, char key) {
    menu->menuType = 0;
    sleep(3);
}

int UpdateMenu(menu_t * menu,char key) {
    switch (menu->menuType) {
        case 0 :
            if (key == 'q' || key == 27) {
                menu->menuType = 4;
                DrawPauseMenu(menu,' ');
            }
            break;
        case 1:
            DrawMainMenu(menu,key);
            break;
        case 2:
            DrawGameSetting(menu,key);
            break;
        case 3:
            JoinServer(menu,key);
            break;
        case 4:
            DrawPauseMenu(menu,key);
            break;
        case 5:
            DrawPlayerCount(menu,key);
            break;
        case 6:
            Disconnect(menu, key);
            break;
        case 7:
            CreateAndJoinServer(menu,key);
            break;
        default:
            return -1;
    }
    return menu->menuType;
}


void CreateButtons(menu_t * menu) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        menu->buttons[i].isActive = false;
    }
    menu->buttons[0].isActive = true;
    menu->menuType = 1;
    menu->gameSize = BIG;
    menu->gameType = LOOP;
    menu->time = 0;
}
