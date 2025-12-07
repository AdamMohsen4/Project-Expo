#include "world.h"
#include "ui.h"

enum { N=0,E=1,S=2,W=3 };

typedef enum {
    ITEM_NONE=0, ITEM_KEY, ITEM_LAMP, ITEM_MATCHES, ITEM_EXTINGUISHER, ITEM_A1, ITEM_A2, ITEM_A3, ITEM_MASTER
} item_t;

struct room {
    const char *name;
    const char *desc;
    int8_t exits[4];
    item_t item;
    uint8_t dark;
    uint8_t locked;
};

#define ROOMS 9
#define INV_MAX 10

static struct room r[ROOMS];
static int8_t cur;
static item_t inv[INV_MAX];
static uint8_t inv_n;
static uint8_t lamp_lit;

static int inv_has(item_t it){
    for (uint8_t i=0;i<inv_n;i++) if (inv[i]==it) return 1;
    return 0;
}
static void inv_add(item_t it){
    if (!it) return;
    if (inv_has(it)) return;
    if (inv_n<INV_MAX) inv[inv_n++]=it;
}
static void inv_remove(item_t it){
    for (uint8_t i=0;i<inv_n;i++){
        if (inv[i]==it){
            for (uint8_t j=i;j+1<inv_n;j++) inv[j]=inv[j+1];
            inv_n--;
            return;
        }
    }
}

void world_init(void){
    r[0]=(struct room){"Entrance","A dusty stone entrance hall. An old LAMP rests on the floor near crumbling pillars. Passages lead east and south into darkness.",{-1,1,7,-1},ITEM_LAMP,0,0};
    r[1]=(struct room){"Hallway","A long corridor with weathered stone walls. To the north, a massive sealed door shows signs of an ancient lock mechanism. Doorways branch east and west.",{2,3,-1,0},ITEM_NONE,0,0};
    r[2]=(struct room){"Locked Chamber","A sealed chamber marked with protective symbols. The air hums with residual energy. An ancient ARTIFACT (A1) hovers on a pedestal, pulsing with blue light.",{6,-1,1,-1},ITEM_A1,0,1};
    r[3]=(struct room){"Supply Closet","A cluttered storage room filled with scattered equipment and broken crates. Among the debris, a brass KEY hangs from a rusted hook on the wall.",{4,5,-1,1},ITEM_KEY,0,0};
    r[4]=(struct room){"Dark Chamber","Complete darkness engulfs everything. Cold stone walls radiate an ancient chill. You sense something valuable is here, but cannot see without light. [ARTIFACT A2 hidden in darkness]",{-1,-1,3,-1},ITEM_A2,1,0};
    r[5]=(struct room){"Maintenance","A technical room with rusty pipes and old machinery. On a metal shelf, you spot some MATCHES still sealed in weathered packaging.",{8,-1,-1,3},ITEM_MATCHES,0,0};
    r[6]=(struct room){"Sealed Gate","The final chamber. A towering gate blocks the exit to freedom. At its center, an intricate mechanism awaits a MASTER KEY to unlock your escape.",{-1,-1,2,-1},ITEM_NONE,0,0};
    r[7]=(struct room){"Hidden Alcove","A narrow alcove hidden from the main path. Ancient markings cover the walls in faded script. Something glints in the shadows - an ARTIFACT (A3) embedded in the stone.",{0,-1,-1,-1},ITEM_A3,0,0};
    r[8]=(struct room){"Control Room",
        "An abandoned control center with dead terminals and darkened screens. A flickering holographic map shows the dungeon layout. " "Beside a cracked console sits a red FIRE EXTINGUISHER — improbably intact.",{-1,-1,5,-1},ITEM_EXTINGUISHER,0,0
    };

    cur=0; inv_n=0; lamp_lit=0;
}

uint8_t world_room_id(void){ return (uint8_t)cur; }
const char *world_room_name(void){ return r[cur].name; }
const char *world_room_desc(void){
    if (r[cur].dark && !lamp_lit) return "It's too dark to see anything.";
    return r[cur].desc;
}

static const char *dirn(int d){ return d==N?"north":d==E?"east":d==S?"south":"west"; }

int world_exit_count(void){
    int n=0; for(int d=0;d<4;d++) if(r[cur].exits[d]!=-1) n++;
    return n;
}
const char *world_exit_name(int exit_index){
    int k=0;
    for(int d=0;d<4;d++) if(r[cur].exits[d]!=-1){
        if(k==exit_index) return dirn(d);
        k++;
    }
    return "";
}
static int exit_idx_to_dir(int exit_index){
    int k=0;
    for(int d=0;d<4;d++) if(r[cur].exits[d]!=-1){
        if(k==exit_index) return d;
        k++;
    }
    return -1;
}

bool world_try_move(int exit_index){
    int d = exit_idx_to_dir(exit_index);
    if(d<0) return false;
    int next = r[cur].exits[d];
    if(next<0 || next>=ROOMS) return false;

    if(next==2 && r[2].locked){
        if(inv_has(ITEM_KEY)){ r[2].locked=0; ui_println("Unlocked."); }
        else { ui_println("Locked. Need key."); return false; }
    }
    if(next==4 && r[4].dark && !lamp_lit){
        ui_println("Too dark. Need a lit lamp.");
        return false;
    }
    cur=(int8_t)next;
    return true;
}

bool world_take_item(void){
    item_t it = r[cur].item;
    if(it==ITEM_NONE){ ui_println("Nothing here."); return false; }
    r[cur].item = ITEM_NONE;
    inv_add(it);

    if(it==ITEM_A1||it==ITEM_A2||it==ITEM_A3){
        int c = inv_has(ITEM_A1)+inv_has(ITEM_A2)+inv_has(ITEM_A3);
        if(c>=3 && !inv_has(ITEM_MASTER)){
            inv_add(ITEM_MASTER);
            ui_println("Artifacts fuse into a MASTER KEY.");
        } else {
            ui_println("Artifact secured.");
        }
    } else {
        ui_println("Taken.");
    }
    return true;
}

void world_use_action(void){
    if(!lamp_lit && inv_has(ITEM_LAMP) && inv_has(ITEM_MATCHES)){
        inv_remove(ITEM_MATCHES);
        lamp_lit=1;
        ui_println("Lamp lit.");
        return;
    }
    ui_println("Nothing happens.");
}

uint8_t world_inventory_count(void){ return inv_n; }

uint16_t world_led_mask(void){
    uint16_t m=0;
    if(inv_has(ITEM_KEY))    m |= (1u<<0);
    if(inv_has(ITEM_LAMP))   m |= (1u<<1);
    if(lamp_lit)             m |= (1u<<2);
    if(inv_has(ITEM_A1))     m |= (1u<<3);
    if(inv_has(ITEM_A2))     m |= (1u<<4);
    if(inv_has(ITEM_A3))     m |= (1u<<5);
    if(inv_has(ITEM_EXTINGUISHER)) m |= (1u<<8);
    if(inv_has(ITEM_MASTER)) m |= (1u<<9);
    return m;
}

bool world_check_win(void){
    return (cur==6) && inv_has(ITEM_MASTER);
}

bool world_has_extinguisher(void){
    return inv_has(ITEM_EXTINGUISHER) ? true : false;
}
