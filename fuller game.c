#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DIALOGUE_LENGTH 500
#define MAX_NAME_LENGTH 16
#define MAX_DESCRIPTION_LENGTH 200
#define MAX_INVENTORY_SIZE 30

// Function prototypes
void initializeWoundedSoldier();
void initializeOldMan();
void intro(Player *player);
void Tavern(Player *player);
void townSquare(Player *player);
void explorePath(Player *player);
void farmersField(Player *player);
void riverCrossing(Player *player);
void bridgeEncounter(Player *player);
void kingEncounter(Player *player);
void lichEncounter(Player *player);
void combat(Player *player, Enemy *enemy, Room *room);
void printInventory(Player *player);
void pickUpItem(Player *player, Room *room);
void interactWithNPC(Player *player, struct NPC *npc);
int checkAndRemoveItem(Player *player, const char *item);
void handleRoomWithEnemies(Player *player, Room *room, Enemy *enemy);


typedef struct {
    char name[MAX_NAME_LENGTH];
    int health;
    int maxHealth;
    int attack;
    int gold;
    char inventory[MAX_INVENTORY_SIZE][MAX_NAME_LENGTH];
    int itemCount;
} Player;

typedef struct Room {
    char name[50];
    char description[200];
    char item[50];
    int hasItem;
    int locked;
    int isCleared; // Flag to indicate if enemies in the room are defeated
} Room;


typedef struct Enemy {
    char name[50];
    int health;
    int isAlive;
} Enemy;

struct Quest {
    int isActive;
    int isCompleted;
    char targetItem[50];
    int rewardGold;
    char description[200];
};

struct NPC {
    char name[50];
    char dialogue[200]; 
    int hasQuest;
    struct Quest quest;
};

typedef struct {
    char name[MAX_NAME_LENGTH];
    int damage;
} Weapon;

Weapon availableWeapons[] = {
    {"Fist", 15},
    {"Short Sword", 30},
    {"Long Sword", 40},
    {"Legendary Sword", 50}
};
const int numWeapons = sizeof(availableWeapons) / sizeof(Weapon);

struct NPC oldMan = {
    .name = "Old Farmer",
    .dialogue = "Hello traveler! I need your help. A troll has stolen a key I was entrusted with by a knight. Please, retrieve it for me!\n\n",
    .hasQuest = 1,
    .quest = {
        .isActive = 0,
        .isCompleted = 0,
        .rewardGold = 50,
    }
};


struct NPC woundedSoldier = {
    .name = "Wounded Soldier",
    .dialogue = "I was part of an elite squad tasked with defeating the Lich King in the graveyard across the river. My companions... they didn't make it. Will you do what we couldn't and kill the Lich?",
    .hasQuest = 1,
    .quest = {
        .isActive = 0,
        .isCompleted = 0,
        .rewardGold = 150,
    }
};




void initializeWoundedSoldier() {
    strcpy(woundedSoldier.quest.targetItem, "Lich's Head");
    strcpy(woundedSoldier.quest.description, "Defeat the Lich King and bring back his head as proof.");
}
void initializeOldMan() {
    strcpy(oldMan.quest.targetItem, "Troll's Key");
    strcpy(oldMan.quest.description, "Retrieve the key stolen by the troll and return it to the old farmer.\n\n");
}


int main() {
    Player player;
    intro(&player);
    srand(time(NULL));
    initializeOldMan();
    initializeWoundedSoldier();
    
    int choice;

    printf("--------------------------------------------------------------------------\n\n");
    
    Tavern(&player);
    
    return 0;
}

void intro(Player *player) {
    printf("\n=================================\n");
    printf("Welcome to the Text Adventure Game\n");
    printf("=================================\n\n");

    char tempName[MAX_NAME_LENGTH + 1];  // +1 for null terminator
    printf("Hello traveler, what is your name? (max 15 characters): ");
    scanf("%15s", tempName);  // Limits input to 15 characters
    while (getchar() != '\n');  // Clear input buffer

    // Copy name to player structure
    strncpy(player->name, tempName, MAX_NAME_LENGTH);
    player->name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null termination

    
    printf("\nWell, %s, good luck.\n\nYou find yourself in a cozy tavern, seemingly waking from a dream\n\n", player->name);
    
    // Initialize player stats
    player->health = 100;
    player->maxHealth = 100;
    player->attack = 10;
    player->gold = 50;
    player->itemCount = 0;
    
    printf("Your starting stats:\n");
    printf("Health: %d/%d\n", player->health, player->maxHealth);
    printf("Attack: %d\n", player->attack);
    printf("Gold: %d\n", player->gold);
    printf("\nPress Enter to continue...");
    getchar();
}

void Tavern(Player *player) {
     int choice;
    
    do{
    printf("\nYou see some other people in the tavern. What would you like to do?\n\n");
    printf("1. Talk to people\n");
    printf("2. Go outside\n\n");
    printf("Your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nThe people around you are too drunk to talk coherently. You're not getting anywhere.\n");
        printf("\nYou decide to start your adventure.\n\n");
        townSquare(player);
    } else if (choice == 2) {
        printf("\nYou decide to start your adventure.\n\n");
        townSquare(player);
        return;
    }
    } while (choice != 2);
}

void townSquare(Player *player) {
    int choice;

    printf("You step into the bustling town of Dunberry. A glistening fountain greets you in the town square, and nearby, you notice a small shop with a large map displayed prominently.\n\n");

    do {
        printf("What would you like to do?\n\n");
        printf("1. Open Inventory\n");
        printf("2. Go to Shop\n");
        printf("3. Inspect Map\n");
        printf("4. Continue on the path\n\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printInventory(player);
                break;
            case 2:
                shop(player);
                break;
            case 3:
                printf("\nYou study the map displayed on the shop wall. It outlines the surrounding regions:\n\n");
                printf("- To the north: a mysterious cave said to hold secrets.\n\n");
                printf("- To the south: a cute farm that needs help.\n\n");
                printf("- To the east: a fast-flowing river splitting the land in two.\n\n");
                printf("- To the northeast: a graveyard where the undead lives.\n\n");
                printf("- To the southeast: towering mountains rumored to be home to ancient creatures.\n\n");
                printf("- Far to the east: the Crystal Kingdom, where the evil lich rules with an iron fist.\n\n");
                printf("The map ends there, not showing anything else.\n\n");
                break;
            case 4:
                printf("\nYou decide to leave and continue on the path.\n\n");
                explorePath(player);
                return;
            default:
                printf("\nInvalid choice. Please choose again.\n\n");
        }
    } while (choice != 4);
}

void shop(Player *player) {
    int itemChoice;

    printf("\nHere are my wares:\n\n");
    printf("1. Health Potion: 10 gold (Restores 20 health)\n\n");
    printf("2. Apple: 2 gold (Restores 5 health)\n\n");
    printf("3. Short Sword: 20 gold (+10 damage in combat)\n\n");
    printf("4. Secret Key: 50 gold (Unlocks a mysterious chest somewhere far away)\n\n");
    printf("5. Damage Potion: 15 gold (Deals 50 damage to an enemy)\n\n");
    printf("6. Exit Shop\n\n");
    printf("Gold: %d\n\n", player->gold);

    while (1) {
        printf("\nGold: %d\n", player->gold);
        printf("What would you like to buy? ");
        scanf("%d", &itemChoice);

        switch (itemChoice) {
            case 1:
                if (player->gold >= 10) {
                    player->gold -= 10;
                    strcpy(player->inventory[player->itemCount], "Health Potion");
                    player->itemCount++;
                    printf("\nYou bought a Health Potion! Remaining gold: %d\n\n", player->gold);
                } else {
                    printf("\nYou don't have enough gold for a Health Potion.\n\n");
                }
                break;
            case 2:
                if (player->gold >= 2) {
                    player->gold -= 2;
                    strcpy(player->inventory[player->itemCount], "Apple");
                    player->itemCount++;
                    printf("\nYou bought an Apple! Remaining gold: %d\n\n", player->gold);
                } else {
                    printf("\nYou don't have enough gold for an Apple.\n\n");
                }
                break;
            case 3:
                if (player->gold >= 20) {
                    player->gold -= 20;
                    strcpy(player->inventory[player->itemCount], "Short Sword");
                    player->itemCount++;
                    printf("\nYou bought a Short Sword! You feel stronger. Remaining gold: %d\n\n", player->gold);

                } else {
                    printf("\nYou don't have enough gold for the Short Sword.\n\n");
                }
                break;
            case 4:
                if (player->gold >= 50) {
                    player->gold -= 50;
                    strcpy(player->inventory[player->itemCount], "Secret Key");
                    player->itemCount++;
                    printf("\nYou bought the Secret Key! You wonder what it unlocks. Remaining gold: %d\n\n", player->gold);
                } else {
                    printf("\nYou don't have enough gold for the Secret Key.\n\n");
                }
                break;
            case 5:
                if (player->gold >= 15) {
                    player->gold -= 15;
                    strcpy(player->inventory[player->itemCount], "Damage Potion");
                    player->itemCount++;
                    printf("\nYou bought a Damage Potion! Enemies beware. Remaining gold: %d\n\n", player->gold);
                } else {
                    printf("\nYou don't have enough gold for the Damage Potion.\n\n");
                }
                break;
            case 6:
                printf("\nThanks for visiting the shop!\n\n");
                return;
            default:
                printf("\nInvalid choice. Please choose a valid item.\n\n");
        
      }
    }
}

void graveyardEncounter(Player *player) {
    Enemy skeletons[3] = {
        {"Skeleton Warrior 1", 30, 1},
        {"Skeleton Warrior 2", 30, 1},
        {"Skeleton Warrior 3", 30, 1}
    };
    int choice;

    printf("\nYou cross the old lady's bridge and arrive at the graveyard.\n");
    printf("The air grows cold, and the ground is littered with crumbling tombstones.\n");
    printf("A dark aura fills the area, and as you step further in, three skeleton warriors rise from the ground, their bones rattling ominously.\n");
    printf("They attack you as a group!\n\n");

    while ((skeletons[0].isAlive || skeletons[1].isAlive || skeletons[2].isAlive) && player->health > 0) {
        printf("\nYour health: %d\n", player->health);
        printf("Skeletons remaining:\n");
        for (int i = 0; i < 3; i++) {
            if (skeletons[i].isAlive) {
                printf("- %s: %d health\n", skeletons[i].name, skeletons[i].health);
            }
        }

        printf("\nWhat will you do?\n");
        printf("1. Attack a skeleton\n");
        printf("2. Open inventory\n");
        printf("3. Attempt to flee\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // Choose which skeleton to attack
                printf("\nChoose your target:\n");
                for (int i = 0; i < 3; i++) {
                    if (skeletons[i].isAlive) {
                        printf("%d. %s\n", i + 1, skeletons[i].name);
                    }
                }
                int target;
                scanf("%d", &target);
                target -= 1;  // Convert to zero-based index

                if (target >= 0 && target < 3 && skeletons[target].isAlive) {
                    printf("\nYou attack %s!\n", skeletons[target].name);
                    skeletons[target].health -= 20;
                    if (skeletons[target].health <= 0) {
                        printf("You have slain %s!\n", skeletons[target].name);
                        skeletons[target].isAlive = 0;
                    } else {
                        printf("%s has %d health remaining.\n", skeletons[target].name, skeletons[target].health);
                    }
                } else {
                    printf("\nInvalid target or the skeleton is already defeated.\n");
                }
                break;
            }

            case 2:
                printInventory(player);
                break;

            case 3: {
                printf("\nYou attempt to flee from the graveyard!\n");
                int escapeNumber, correctEscapeNumber = rand() % 3 + 1;
                printf("Pick a number (1-3): ");
                scanf("%d", &escapeNumber);

                if (escapeNumber == correctEscapeNumber) {
                    printf("\nYou successfully escape the graveyard!\n");
                    return;
                } else {
                    printf("\nThe skeletons block your escape, forcing you to fight!\n");
                }
                break;
            }

            default:
                printf("\nInvalid choice! The skeletons take advantage of your hesitation!\n");
                break;
        }

        // Skeletons' turn to attack
        printf("\nThe skeletons attack!\n");
        for (int i = 0; i < 3; i++) {
            if (skeletons[i].isAlive) {
                printf("%s attacks you for 10 damage!\n", skeletons[i].name);
                player->health -= 10;
                if (player->health <= 0) {
                    printf("\nYou have been defeated by the skeletons. Game Over.\n");
                    exit(0);
                }
            }
        }
    }

    if (!skeletons[0].isAlive && !skeletons[1].isAlive && !skeletons[2].isAlive) {
    printf("\nYou have defeated all the skeletons!\n");
    printf("The graveyard grows silent, but you feel an ominous presence deeper within...\n\n");
    graveyardSearch(player);

}
}

void explorePath(Player *player) {
    int choice;

    printf("\nYou get to the fork in the road.\n\n");
    printf("1. Take the North path, which looks dark and narrow.\n\n");
    printf("2. Take the East path, where you can hear some running water.\n\n");
    printf("3. Take the South path, which looks well worn.\n\n");
    printf("4. Go to Dunberry.\n\n");
    printf("What do you choose? ");
    scanf("%d", &choice);

    if (choice == 2) {
        riverCrossing(player);
    } else if (choice == 1) {
        caveEncounter(player);
    } else if (choice == 3) {
        farmersField(player);
    } else if (choice == 4) {
        townSquare(player);
    }else {
        printf("\nInvalid choice. Try again.\n\n");
    }
}

void riverCrossing(Player *player) {
    int choice;

    printf("\nYou reach a fast-flowing river.\n\n");
    printf("You see a rotted tree fallen across the river.\n\n");
    printf("What would you like to do.\n\n");
    printf("1. try to walk arcross the tree.\n\n");
    printf("2. Look for a bridge or some other ways to cross.\n\n");
    printf("What do you choose? ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nThe tree snaps, dropping you into the water and forcing you downstream.\n\n");
        player->health -= 20;
        printf("You lose 20 health. Current health: %d\n\n", player->health);
        printf("You swim back to shore and claw your way onto dry land.\n\n");
        printf("You stand up and dry yourself off.\n\n");
        farmersField(player);
    } else if (choice == 2) {
        printf("\nYou look around but you don't see any other way to cross maybe one of the other paths has a way.\n");
        printf("You turn around back to the main path.\n");
        explorePath(player);
    } else {
        printf("\nInvalid choice. You lose your way and wander back to the forest entrance.\n\n");
        explorePath(player);
    }
}

void caveEncounter(Player *player, Room *room) {
    // Ensure the room pointer is valid
    if (!room) {
        printf("\nError: Room data not found.\n");
        return;
    }

    printf("\nYou enter the %s.\n", room->name);

    // Adjust behavior based on the room's state
    switch (room->isCleared) {
        case 0: // Room not cleared
            printf("\n%s\n", room->description);

            // Spawn and engage the enemy
            Enemy goblin = {"Goblin", 30, 1};
            printf("\nA %s appears!\n", goblin.name);
            combat(player, &goblin, room);

            // If enemy is defeated, mark the room as cleared
            if (!goblin.isAlive) {
                room->isCleared = 1;
                printf("\nThe %s is now safe.\n", room->name);
            }
            break;

        case 1: // Room cleared
            printf("\nThe %s is quiet. The enemies here have already been defeated.\n", room->name);

            if (room->hasItem) {
                printf("\nYou notice something on the ground: %s.\n", room->item);
                printf("Do you want to pick it up? (1: Yes, 2: No): ");
                int choice;
                scanf("%d", &choice);

                if (choice == 1) {
                    pickUpItem(player, room);
                } else {
                    printf("\nYou leave the item untouched.\n");
                }
            }
            break;

        default:
            printf("\nThe room state is unknown. Please report this bug!\n");
            break;
    }
}


void bridgeEncounter(Player *player) {
    Enemy bridgeTroll = {"Bridge Troll", 50, 1};
    Room bridgeRoom = {
        .name = "Bridge Crossing",
        .description = "A pristine wooden bridge surrounded by a bright clearing.",
        .item = "Troll's Key",
        .hasItem = 1,
        .locked = 0
    };

    int choice;

    printf("\nYou step into a bright clearing as you come upon a pristine wooden bridge.\n");
    printf("The surrounding area is serene, but the air feels heavy with a sense of foreboding.\n\n");

    do {
        printf("What would you like to do?\n");
        printf("1. Inspect the bridge\n");
        printf("2. Look around the area\n");
        printf("3. Go back to the farm\n");
        printf("4. Step onto the bridge\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nYou examine the bridge closely. It appears to be well-maintained, with sturdy wooden planks and strong ropes.\n");
                printf("However, there are faint signs of claw marks and scorch marks near the edges, hinting at danger.\n");
                break;

            case 2:
                printf("\nYou look around the area and notice some bushes and trees near the bridge.\n");
                printf("After searching for a moment, you find nothing of interest. The area seems clear.\n");
                break;

            case 3:
                printf("\nYou decide to go back to the farm. The clearing behind you feels safer as you retreat.\n");
                farmersField(player);
                return;

            case 4:
                printf("\nYou cautiously step onto the bridge.\n");
                printf("As you do, a massive figure emerges from beneath the bridge! It's a Bridge Troll!\n");
                printf("The troll growls, \"Pay the toll or face my wrath!\"\n\n");
                trollEncounter(player, &bridgeTroll, &bridgeRoom);
                bridgeEncounter(player);
                return;

            default:
                printf("\nInvalid choice. Please choose again.\n");
        }
    } while (1);
}

void trollEncounter(Player *player, Enemy *bridgeTroll, Room *bridgeRoom) {
    int choice;

    do {
        printf("\nWhat would you like to do?\n");
        printf("1. Fight the Bridge Troll\n");
        printf("2. Attempt to negotiate\n");
        printf("3. Try to flee\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nYou prepare to fight the troll on the narrow bridge!\n");
                combat(player, bridgeTroll);
                if (!bridgeTroll->isAlive) {
                    printf("\nThe troll lets out a final roar before collapsing. You carefully step over its massive body.\n");

                    // Check if the troll has an item (the key) to collect
                    if (bridgeRoom->hasItem) {
                        printf("As you stand victourious, you see something taped around the trolls neck. What do you do?\n\n");
                        printf("1. Inspect\n\n");
                        printf("2. Go back to bridge entrance\n\n");
                        printf("3. Head up the mountain\n\n");
                        scanf("%d", &choice);
                        if (choice == 1){
                        pickUpItem(player, bridgeRoom);
                        printf("\nYou found the Troll's Key on its body! You can talk to the old man now!\n\n");
                        printf("What do you do now?\n\n");
                        printf("1. Go back to bridge entrance\n\n");
                        printf("2. Head up the mountain\n\n");
                        scanf("%d", &choice);
                        if (choice == 1){
                            bridgeEncounter(player);
                            return;
                        } else if (choice == 2){
                            mountainsEncounter(player);
                         return;
                        } else {
                            printf("Invalid Number, try again");
                        }
                        } else if (choice == 2){
                            bridgeEncounter(player);
                            return;
                        } else if (choice == 3){
                            mountainsEncounter(player);
                         return;
                        }

                    } else {
                        printf("\nThe troll seems to have nothing of value left.\n");
                    }
                }
                return;

            case 2:
                printf("\nYou try to reason with the troll.\n");
                if (player->gold >= 20) {
                    printf("The troll considers your offer and says, \"Fine. 20 gold and I'll let you pass.\"\n");
                    printf("Do you want to pay the toll? (1 for Yes, 2 for No): ");
                    int payChoice;
                    scanf("%d", &payChoice);
                    if (payChoice == 1) {
                        player->gold -= 20;
                        printf("\nYou pay the toll and the troll steps aside, allowing you to pass.\n");
                        printf("Remaining gold: %d\n", player->gold);
                        return;
                    } else {
                        printf("\nThe troll snarls, \"Then you shall face my wrath!\"\n");
                        combat(player, bridgeTroll);
                    }
                } else {
                    printf("\nThe troll laughs at your empty pockets and attacks you!\n");
                    combat(player, bridgeTroll);
                }
                break;

            case 3:
                printf("\nYou attempt to flee across the bridge!\n");
                int fleeNumber, correctFleeNumber = rand() % 3 + 1;
                printf("Pick a number (1-3): ");
                scanf("%d", &fleeNumber);

                if (fleeNumber == correctFleeNumber) {
                    printf("\nYou sprint past the troll and make it across the bridge safely!\n");
                    mountainsEncounter(player);
                    return;
                } else {
                    printf("\nThe troll blocks your escape and roars, \"Coward! Face me!\"\n");
                    combat(player, bridgeTroll);
                }
                break;

            default:
                printf("\nInvalid choice. The troll glares at you menacingly.\n");
        }

        if (player->health <= 0) {
            printf("\nThe troll has defeated you. Game Over.\n");
            exit(0);
        }
    } while (1);
}


int findBestWeapon(Player *player) {
    int bestDamage = 0;
    for (int i = 0; i < player->itemCount; i++) {
        for (int j = 0; j < numWeapons; j++) {
            if (strcmp(player->inventory[i], availableWeapons[j].name) == 0) {
                if (availableWeapons[j].damage > bestDamage) {
                    bestDamage = availableWeapons[j].damage;
                }
            }
        }
    }
    return bestDamage > 0 ? bestDamage : availableWeapons[0].damage; // Default to "Fist" if no weapon
}


void mountainsEncounter(Player *player) {
    int choice;
    int dragonAwake = 0; // Track if the dragon wakes up (0 = sleeping, 1 = awake)

    printf("\nYou climb the steep, rocky path into the towering mountains. The air grows thin and cold as you ascend.\n");
    printf("At the peak, you discover a magnificent cave glittering with gems of every color. The walls seem to glow softly, illuminated by the faint light filtering in.\n");
    printf("In the center of the cavern lies a massive Crystal Dragon, its scales shimmering like diamonds. It is sleeping soundly, but its presence fills the air with an aura of power.\n\n");

    do {
        printf("What would you like to do?\n");
        printf("1. Examine the surroundings\n");
        printf("2. Attempt to steal a gem from the lair\n");
        printf("3. Try to sneak past the dragon\n");
        printf("4. Walk down the northern path\n");
        printf("5. Leave the mountain quietly\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nYou take a closer look at the cavern.\n");
                printf("The walls are encrusted with precious gems, and in the far corner, you spot a large golden chest partially hidden beneath rubble.\n");
                printf("The sleeping dragon occasionally lets out a soft growl, its claws twitching slightly in its slumber.\n\n");
                break;

            case 2: {
                printf("\nYou cautiously approach a cluster of glittering gems near the dragon.\n");
                printf("Your hands tremble as you reach out to grab one...\n");

                // Determine if the dragon wakes up (50% chance)
                int wakeChance = rand() % 2;
                if (wakeChance == 0) {
                    printf("\nYou manage to snatch a gem without waking the dragon! You quickly add it to your inventory.\n");
                    if (player->itemCount < MAX_INVENTORY_SIZE) {
                        strcpy(player->inventory[player->itemCount], "Crystal Gem");
                        player->itemCount++;
                    } else {
                        printf("Your inventory is full! You have to leave the gem behind.\n");
                    }
                } else {
                    printf("\nThe dragon stirs and opens its gleaming eyes, locking its gaze on you. The Crystal Dragon roars furiously, and the cavern trembles!\n");
                    dragonAwake = 1;
                }
                break;
            }

            case 3:
                printf("\nYou decide to sneak past the dragon and explore the far side of the lair.\n");

                // Determine if the dragon wakes up (50% chance)
                int sneakChance = rand() % 2;
                if (sneakChance == 0) {
                    printf("\nYou successfully sneak past the dragon and find a hidden passage leading deeper into the mountain.\n");
                    printf("You decide not to disturb the dragon further and quietly return to the entrance.\n");
                } else {
                    printf("\nThe dragon's eyes snap open as you accidentally knock over a small pile of gems. It rises, its crystalline wings spreading wide as it roars at you.\n");
                    dragonAwake = 1;
                }
                break;

            case 4:
                if (dragonAwake) {
                    printf("\nThe Crystal Dragon blocks your path, forcing you to deal with it before leaving.\n");
                } else {
                    printf("\nYou carefully leave the lair and head down the northern path.\n");
                    kingEncounter(player);
                    return; // Exit the encounter
                }

            case 5:
                printf("\nYou decide to leave the mountain quietly and return to where you started.\n");
                explorePath(player); // Go back to the general exploration function
                return;

            default:
                printf("\nInvalid choice. Please choose again.\n");
        }

        // If the dragon wakes up
        if (dragonAwake) {
            printf("\nThe Crystal Dragon stands before you, its glowing scales pulsing with power.\n");
            printf("Its voice booms, \"WHO DARES TO DISTURB MY SLUMBER?\"\n");

            printf("\nWhat will you do?\n");
            printf("1. Apologize and beg for mercy\n");
            printf("2. Offer gold to appease the dragon\n");
            printf("3. Attempt to fight the dragon\n");
            printf("4. Flee the lair\n");
            printf("Your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("\nYou fall to your knees and apologize profusely.\n");
                    printf("The dragon stares at you for a moment before huffing, \"LEAVE, AND NEVER RETURN.\" You quickly retreat down the mountain.\n");
                    return;

                case 2:
                    printf("\nYou offer the dragon 50 gold in hopes of appeasing its wrath.\n");
                    if (player->gold >= 50) {
                        player->gold -= 50;
                        printf("The dragon snorts, \"A MEAGER GIFT, BUT ACCEPTABLE. NOW BE GONE.\"\n");
                        printf("You carefully retreat down the mountain. Remaining gold: %d\n", player->gold);
                        return;
                    } else {
                        printf("The dragon roars, \"YOU DARE OFFER ME NOTHING?\" It prepares to attack!\n");
                        combat(player, &(Enemy){"Crystal Dragon", 100, 1});
                    }
                    break;

                case 3:
                    printf("\nYou prepare to fight the Crystal Dragon!\n");
                    combat(player, &(Enemy){"Crystal Dragon", 100, 1});
                    if (player->health > 0) {
                        printf("\nYou have defeated the Crystal Dragon! Its body shatters into shards of crystal, leaving behind a massive gem.\n");
                        if (player->itemCount < MAX_INVENTORY_SIZE) {
                            strcpy(player->inventory[player->itemCount], "Dragon Crystal");
                            player->itemCount++;
                        } else {
                            printf("Your inventory is full! You leave the crystal behind.\n");
                        }
                        printf("\nWith the dragon defeated, you explore the lair and find a treasure chest filled with gold.\n");
                        player->gold += 200;
                        printf("You receive 200 gold! Current gold: %d\n", player->gold);
                    } else {
                        printf("\nThe Crystal Dragon has defeated you. Game Over.\n");
                        exit(0);
                    }
                    return;

                case 4:
                    printf("\nYou attempt to flee, but the dragon roars and chases after you!\n");
                    combat(player, &(Enemy){"Crystal Dragon", 100, 1});
                    break;

                default:
                    printf("\nInvalid choice. The dragon looms over you menacingly.\n");
            }
        }
    } while (1);
}


void oldLadyBridge(Player *player) {
    int choice;
    struct NPC oldLady = {
        .name = "Old Lady",
        .dialogue = "Ah, traveler, you seem brave enough to face the dangers of this land. Beware, for the mountains hold a fearsome dragon. And just up ahead is the lich king, he kills anyone in his path, be carful.",
        .hasQuest = 0 // No quest from the old lady
    };

    printf("\nYou approach a rickety wooden bridge swaying gently in the wind. Standing at the center is a frail old woman wrapped in a tattered cloak.\n");
    printf("As you near her, she gazes at you with piercing, wise eyes.\n\n");

    do {
        printf("What would you like to do?\n");
        printf("1. Talk to the old lady\n");
        printf("2. Cross the bridge silently\n");
        printf("3. Turn back\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nYou approach the old lady.\n");
                interactWithNPC(player, &oldLady);
                printf("She smiles faintly and adds, \"Good luck, %s.\"\n\n", player->name);
                break;

            case 2:
                printf("\nYou decide to cross the bridge silently, nodding politely to the old lady as you pass.\n");
                printf("Her voice follows you: \"Remember, %s ,not all battles are won by strength alone.\"\n", player->name);
                printf("\nYou make it across the bridge safely.\n\n");
                graveyardEncounter(player);
                return;

            case 3:
                printf("\nYou decide to turn back, feeling unprepared for what lies ahead.\n");
                return;

            default:
                printf("\nInvalid choice. Please choose again.\n");
        }
    } while (1);
}

void combat(Player *player, Enemy *enemy, Room *room) {
    if (!enemy->isAlive) {
        printf("\nThe %s is already defeated.\n", enemy->name);
        return;
    }

    printf("\nA %s lunges at you!\n", enemy->name);

    while (enemy->isAlive && player->health > 0) {
        printf("\nYour health: %d | %s's health: %d\n", player->health, enemy->name, enemy->health);
        printf("1. Attack\n2. Open Inventory\n3. Flee\nYour choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Attack
                printf("\nYou attack the %s!\n", enemy->name);
                int damage = findBestWeapon(player);
                enemy->health -= damage;
                printf("You dealt %d damage!\n", damage);

                if (enemy->health <= 0) {
                    printf("\nYou defeated the %s!\n", enemy->name);
                    enemy->isAlive = 0;

                    if (room) {
                        room->isCleared = 1; // Mark the room as cleared
                    }
                }
                break;

            case 2: // Open Inventory
                printInventory(player);
                break;

            case 3: // Flee
                printf("\nYou flee from the battle!\n");
                return;

            default:
                printf("\nInvalid choice. Try again.\n");
        }

        // Enemy attacks if still alive
        if (enemy->isAlive) {
            printf("\nThe %s attacks you!\n", enemy->name);
            int enemyDamage = 15;
            player->health -= enemyDamage;
            printf("You take %d damage! Current health: %d\n", enemyDamage, player->health);

            if (player->health <= 0) {
                printf("\nYou have been defeated by the %s. Game Over.\n", enemy->name);
                exit(0);
            }
        }
    }
}


void printInventory(Player *player) {
    if (player->itemCount == 0) {
        printf("\nYour inventory is empty.\n\n");
    } else {
        printf("\nYour inventory:\n\n");
        for (int i = 0; i < player->itemCount; i++) {
            printf("- %s\n\n", player->inventory[i]);
        }
    }
}

void pickUpItem(Player *player, Room *room) {
    if (room->hasItem) {
        if (player->itemCount < MAX_INVENTORY_SIZE) {
            strcpy(player->inventory[player->itemCount], room->item);
            player->itemCount++;
            printf("\nYou picked up %s.\n", room->item);
            room->hasItem = 0;
            room->item[0] = '\0'; // Clear the item
        } else {
            printf("\nYour inventory is full! You cannot pick up %s.\n", room->item);
        }
    } else {
        printf("\nThere is nothing to pick up here.\n");
    }
}

void interactWithNPC(Player *player, struct NPC *npc) {
    printf("\n%s says: %s\n", npc->name, npc->dialogue);

    if (npc->hasQuest) {
        if (npc->quest.isActive && !npc->quest.isCompleted) {
            // Check if the player has the required item
            if (checkAndRemoveItem(player, npc->quest.targetItem)) {
                // Complete the quest
                printf("\nYou present the %s to %s.\n", npc->quest.targetItem, npc->name);
                printf("%s smiles and says, 'Thank you! You've helped me greatly.'\n", npc->name);

                player->gold += npc->quest.rewardGold; // Reward the player
                printf("\nYou received %d gold! Current gold: %d\n", npc->quest.rewardGold, player->gold);

                npc->quest.isCompleted = 1; // Mark quest as completed
                npc->quest.isActive = 0;   // Deactivate quest
                strncpy(npc->dialogue, "Thank you for completing the quest!", sizeof(npc->dialogue));
            } else {
                // Player does not have the item
                printf("\n'%s says: \"Remember, bring me the %s as proof of your success.\"\n", npc->name, npc->quest.targetItem);
            }
        } else if (!npc->quest.isActive && !npc->quest.isCompleted) {
            // Offer the quest if not active and not completed
            printf("\nThis NPC has a quest for you: %s\n", npc->quest.description);
            printf("Reward: %d gold\n", npc->quest.rewardGold);
            printf("Will you accept the quest? (1: Yes, 2: No): ");
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                npc->quest.isActive = 1;
                printf("\nQuest accepted: %s\n", npc->quest.description);
            } else {
                printf("\nYou declined the quest.\n");
            }
        } else if (npc->quest.isCompleted) {
            // Quest already completed
            printf("\n%s nods at you with respect and says, 'Thank you again for your help.'\n", npc->name);
        }
    } else {
        printf("\n%s has nothing more to ask of you.\n", npc->name);
    }
}

int checkAndRemoveItem(Player *player, const char *item) {
    for (int i = 0; i < player->itemCount; i++) {
        if (strcmp(player->inventory[i], item) == 0) {
            // Shift remaining items to remove the found item
            for (int j = i; j < player->itemCount - 1; j++) {
                strcpy(player->inventory[j], player->inventory[j + 1]);
            }
            player->itemCount--; // Reduce item count
            printf("\nYou handed over the %s.\n", item);
            return 1; // Item found and removed
        }
    }
    printf("\nYou don't have the required item: %s\n", item);
    return 0; // Item not found
}

void farmersField(Player *player) {
    static Room farmField = {
        .name = "Farmer's Field",
        .description = "A vast field with golden wheat swaying gently in the breeze. An old farmhouse stands nearby.",
        .item = "",
        .hasItem = 0,
        .locked = 0,
        .isCleared = 0 // Initially not cleared
    };

    static Enemy scarecrow = {"Enchanted Scarecrow", 40, 1}; // Enemy in this room, if any

    int choice;

    // Adjust behavior based on the room's state
    switch (farmField.isCleared) {
        case 0: // Room not cleared
            printf("\nYou arrive at the %s. %s\n", farmField.name, farmField.description);

            // Check if the Old Farmer's quest is active or completed
            if (oldMan.quest.isCompleted) {
                printf("\nThe farmer smiles warmly: \"Thank you again for retrieving the key. I have nothing else for you now.\"\n");
            } else {
                printf("\nAn old man is trying to get your attention.\n");
                printf("\nWhat would you like to do?\n");
                printf("1. Talk to the farmer\n");
                printf("2. Ignore the farmer and continue towards the bridge\n");
                printf("3. Go back to the main path\n");
                printf("Your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        // Interact with the farmer (NPC)
                        interactWithNPC(player, &oldMan);

                        // After interaction, check if the quest is active and the player hasn't already fought the scarecrow
                        if (oldMan.quest.isActive && !farmField.isCleared) {
                            // Enemy encounter (optional, if you want an enemy here)
                            printf("\nAs you finish speaking with the farmer, an %s lurches towards you from the field!\n", scarecrow.name);
                            combat(player, &scarecrow, &farmField);

                            if (!scarecrow.isAlive) {
                                farmField.isCleared = 1; // Mark the field as cleared
                                printf("\nThe %s collapses into a heap of straw.\n", scarecrow.name);
                            }
                        }
                        break;

                    case 2:
                        // Continue towards the bridge (troll encounter)
                        printf("\nYou decide to head east towards the troll's bridge.\n");
                        bridgeEncounter(player);
                        return;

                    case 3:
                        // Go back to the main path
                        printf("\nYou decide to return to the main path.\n");
                        explorePath(player);
                        return;

                    default:
                        printf("\nInvalid choice. Please try again.\n");
                }
            }
            break;

        case 1: // Room cleared
            printf("\nYou return to the %s. The field is peaceful, and the farmer is tending to his crops.\n", farmField.name);

            if (oldMan.quest.isCompleted) {
                printf("\nThe farmer waves at you: \"Thanks again for your help!\"\n");
            } else if (oldMan.quest.isActive) {
                printf("\nThe farmer reminds you: \"The troll lives under the bridge on the river to the east. Be careful, traveler!\"\n");
            } else {
                printf("\nAn old man is working in the field.\n");
            }

            printf("\nWhat would you like to do?\n");
            printf("1. Talk to the farmer\n");
            printf("2. Continue towards the bridge\n");
            printf("3. Go back to the main path\n");
            printf("Your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    interactWithNPC(player, &oldMan);
                    break;

                case 2:
                    printf("\nYou decide to head east towards the troll's bridge.\n");
                    bridgeEncounter(player);
                    return;

                case 3:
                    printf("\nYou decide to return to the main path.\n");
                    explorePath(player);
                    return;

                default:
                    printf("\nInvalid choice. Please try again.\n");
            }
            break;

        default:
            printf("\nThe state of the farm is unknown. Please report this bug!\n");
            break;
    }
}

void unlockRoom(Player *player, Room *room) {
    if (room->locked) {
        for (int i = 0; i < player->itemCount; i++) {
            if (strcmp(player->inventory[i], "Secret Key") == 0) {
                room->locked = 0;
                printf("\nYou used the Secret Key to unlock the %s.\n\n", room->name);
                return;
            }
        }
        printf("\nYou don't have the key to unlock the %s.\n\n", room->name);
    } else {
        printf("\nThe %s is already unlocked.\n\n", room->name);
    }
}

void checkActiveQuests(Player *player, struct NPC *npc) {
    if (npc->quest.isActive) {
        printf("\nActive Quest: %s\n", npc->quest.description);
        printf("Reward: %d gold\n", npc->quest.rewardGold);
    } else {
        printf("No active Quests");
    }
}

void kingdomSquare(Player *player) {
    int choice;
    printf("\nYou step into the Kingdom Square, a vibrant green space filled with blooming flowers, neatly trimmed hedges, and sparkling fountains.\n");
    printf("The air is fragrant with the scent of roses, and the square is bustling with activity as nobles and citizens stroll through, enjoying the luxury.\n");
    printf("A grand staircase to the east leads to the Throne Room, while paths to the north and west lead to the Graveyard and the Mountains, respectively.\n\n");

    do {
        printf("Where would you like to go?\n");
        printf("1. Enter the Throne Room\n");
        printf("2. Head to the Graveyard\n");
        printf("3. Return to the Mountains\n");
        printf("4. Stay in the Kingdom Square a little longer\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nYou walk up the grand staircase and push open the heavy doors to the Throne Room.\n");
                kingEncounter(player);
                return;
            case 2:
                printf("\nYou follow the cobblestone path north, which grows darker and quieter as you approach the Graveyard.\n");
                graveyardEncounter(player);
                return;
            case 3:
                printf("\nYou retrace your steps west, climbing back into the rugged Mountains.\n");
                mountainsEncounter(player);
                return;
            case 4:
                printf("\nYou take a moment to soak in the beauty of the Kingdom Square. The lavish surroundings remind you of the grandeur of this kingdom.\n");
                break;
            default:
                printf("\nInvalid choice. Please choose again.\n");
        }
    } while (1);
}

void graveyardSearch(Player *player) {
    static Room graveyard = {
        .name = "Graveyard",
        .description = "An eerie graveyard with broken tombstones and an oppressive silence.",
        .item = "Ancient Relic",
        .hasItem = 1,
        .locked = 0,
        .isCleared = 0 // Initially not cleared
    };

    static Enemy skeletons[3] = {
        {"Skeleton Warrior 1", 30, 1},
        {"Skeleton Warrior 2", 30, 1},
        {"Skeleton Warrior 3", 30, 1}
    };

    int choice;

    // Check if the graveyard is cleared
    if (!graveyard.isCleared) {
        printf("\nYou cautiously enter the %s. %s\n", graveyard.name, graveyard.description);

        printf("\nAs you explore, three Skeleton Warriors rise from the ground!\n");

        // Combat loop for multiple enemies
        for (int i = 0; i < 3; i++) {
            if (skeletons[i].isAlive) {
                combat(player, &skeletons[i], &graveyard);
            }
        }

        // Check if all skeletons are defeated
        int allDefeated = 1;
        for (int i = 0; i < 3; i++) {
            if (skeletons[i].isAlive) {
                allDefeated = 0;
                break;
            }
        }

        if (allDefeated) {
            graveyard.isCleared = 1;
            printf("\nThe graveyard falls silent. All enemies have been defeated.\n");

            // Allow item collection if available
            if (graveyard.hasItem) {
                printf("\nYou notice something glimmering on the ground: %s.\n", graveyard.item);
                printf("Do you want to pick it up? (1: Yes, 2: No): ");
                scanf("%d", &choice);

                if (choice == 1) {
                    pickUpItem(player, &graveyard);
                } else {
                    printf("\nYou leave the %s behind.\n", graveyard.item);
                }
            }
        }
    } else {
        // Graveyard already cleared
        printf("\nThe graveyard is eerily quiet. The Skeleton Warriors have been defeated.\n");

        if (graveyard.hasItem) {
            printf("\nYou notice something glimmering on the ground: %s.\n", graveyard.item);
            printf("Do you want to pick it up? (1: Yes, 2: No): ");
            scanf("%d", &choice);

            if (choice == 1) {
                pickUpItem(player, &graveyard);
            } else {
                printf("\nYou leave the %s behind.\n", graveyard.item);
            }
        }
    }

    // After combat or exploration, search the graveyard for potions
    printf("\nYou begin searching the graveyard for anything useful.\n");

    int foundHealthPotions = 2;
    int foundHarmingPotion = 1;

    if (foundHealthPotions > 0) {
        printf("You find %d Health Potions among the ruins.\n", foundHealthPotions);
        for (int i = 0; i < foundHealthPotions && player->itemCount < MAX_INVENTORY_SIZE; i++) {
            strcpy(player->inventory[player->itemCount], "Health Potion");
            player->itemCount++;
        }
    }

    if (foundHarmingPotion > 0 && player->itemCount < MAX_INVENTORY_SIZE) {
        printf("You find a Potion of Harming among the ruins.\n");
        strcpy(player->inventory[player->itemCount], "Potion of Harming");
        player->itemCount++;
    }

    // Options for further exploration
    printf("\nAfter finishing your search, you look around to decide your next move.\n");
    printf("Options:\n");
    printf("1. Return to the Old Lady Bridge\n");
    printf("2. Head to Kingdom Square\n");
    printf("3. Approach the giant tombstone at the back of the graveyard\n");
    printf("Your choice: ");
    
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\nYou make your way back to the Old Lady Bridge.\n");
            oldLadyBridge(player);
            break;
        case 2:
            printf("\nYou head towards the Kingdom Square.\n");
            kingdomSquare(player);
            break;
        case 3:
            printf("\nYou cautiously approach the giant tombstone at the back of the graveyard.\n");
            lichEncounter(player);
            break;
        default:
            printf("\nInvalid choice. You stand still, unsure where to go.\n");
            graveyardSearch(player);
    }
}


void lichEncounter(Player *player) {
    static Room lichRoom = {
        .name = "Lich's Tomb",
        .description = "A massive tombstone adorned with glowing runes. The air is thick with dread.",
        .item = "Lich's Head",
        .hasItem = 1,
        .locked = 0,
        .isCleared = 0 // Initially not cleared
    };

    static Enemy lich = {"Lich King", 120, 1}; // Enemy in this room

    int choice;

    // Check if the room is cleared
    if (lichRoom.isCleared) {
        printf("\nThe tomb is eerily quiet. The Lich King has been defeated.\n");

        if (lichRoom.hasItem) {
            printf("\nYou notice the Lich's Head lying on the ground.\n");
            printf("Do you want to pick it up? (1: Yes, 2: No): ");
            scanf("%d", &choice);

            if (choice == 1) {
                pickUpItem(player, &lichRoom);
                printf("\nYou acquired the Lich's Head. It feels heavy with dark energy.\n");
            } else {
                printf("\nYou leave the Lich's Head untouched.\n");
            }
        }

        return;
    }

    // Engage with the Lich King
    printf("\nYou approach the %s. The %s rises from its grave, emanating a terrifying aura.\n", lichRoom.name, lich.name);
    printf("\"Foolish mortal,\" the Lich growls. \"You dare disturb my domain? Prepare to perish!\"\n");

    do {
        printf("\nWhat would you like to do?\n");
        printf("1. Fight the Lich King\n");
        printf("2. Attempt to flee\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Fight the Lich King
                printf("\nYou prepare to fight the Lich King!\n");
                combat(player, &lich, &lichRoom);

                if (!lich.isAlive) {
                    printf("\nThe Lich King lets out a final, piercing scream before collapsing into a heap of ashes.\n");

                    // Mark the room as cleared
                    lichRoom.isCleared = 1;

                    if (lichRoom.hasItem) {
                        pickUpItem(player, &lichRoom);
                        printf("\nYou found the Lich's Head among the remains. It might be important.\n");
                    }
                }
                return;

            case 2: // Attempt to flee
                printf("\nYou attempt to flee from the tomb!\n");
                int fleeNumber = rand() % 3 + 1;
                int playerChoice;
                printf("Pick a number (1-3): ");
                scanf("%d", &playerChoice);

                if (playerChoice == fleeNumber) {
                    printf("\nYou escape the tomb safely, but the Lich's presence still looms.\n");
                    return;
                } else {
                    printf("\nThe Lich blocks your escape with a wall of dark energy! You must fight.\n");
                }
                break;

            default:
                printf("\nInvalid choice. The Lich King looms over you menacingly.\n");
        }
    } while (1);
}


void kingEncounter(Player *player) {
    static Room throneRoom = {
        .name = "Throne Room",
        .description = "A grand chamber adorned with golden chandeliers and crimson carpets. The King's presence is overwhelming.",
        .item = "King's Sword",
        .hasItem = 1,
        .locked = 0,
        .isCleared = 0 // Initially not cleared
    };

    static Enemy theKing = {"Mighty King", 175, 1}; // Enemy in this room

    int choice;

    // Check if the room is cleared
    if (throneRoom.isCleared) {
        printf("\nThe throne room is eerily quiet. The Mighty King has been defeated.\n");

        if (throneRoom.hasItem) {
            printf("\nYou notice the King's Sword glimmering on the ground.\n");
            printf("Do you want to pick it up? (1: Yes, 2: No): ");
            scanf("%d", &choice);

            if (choice == 1) {
                pickUpItem(player, &throneRoom);
                printf("\nYou acquired the King's Sword! It feels incredibly powerful.\n");
            } else {
                printf("\nYou leave the King's Sword untouched.\n");
            }
        }

        return;
    }

    // Engage with the King
    printf("\nYou enter the %s. The %s glares at you with unwavering resolve.\n", throneRoom.name, theKing.name);

    do {
        printf("\nWhat would you like to do?\n");
        printf("1. Fight the Mighty King\n");
        printf("2. Attempt to negotiate\n");
        printf("3. Try to flee\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Fight the King
                printf("\nYou prepare to fight the Mighty King in his throne room!\n");
                combat(player, &theKing, &throneRoom);

                if (!theKing.isAlive) {
                    printf("\nThe Mighty King has fallen! The kingdom's fate is now in your hands.\n");

                    // Mark the room as cleared and allow item collection
                    throneRoom.isCleared = 1;

                    if (throneRoom.hasItem) {
                        pickUpItem(player, &throneRoom);
                        printf("\nYou found the King's Sword! It looks extremely powerful...\n");
                    }
                }
                return;

            case 2: // Negotiate with the King
                printf("\nYou try to negotiate with the King.\n");
                if (player->gold >= 100) {
                    printf("The King considers your offer and says, \"Fine. 100 gold and I'll let you live.\"\n");
                    printf("Do you want to pay the King? (1: Yes, 2: No): ");
                    int payChoice;
                    scanf("%d", &payChoice);

                    if (payChoice == 1) {
                        player->gold -= 100;
                        printf("\nYou pay the toll and the King steps aside, allowing you to leave the castle with your life.\n");
                        printf("Remaining gold: %d\n", player->gold);

                        // Avoid fighting and mark the room as cleared
                        throneRoom.isCleared = 1;
                        return;
                    } else {
                        printf("\nThe King bellows, \"Then you shall face my wrath!\"\n");
                    }
                } else {
                    printf("\nThe King laughs at your empty pockets and attacks you!\n");
                }

                // Fight after failed negotiation
                combat(player, &theKing, &throneRoom);

                if (!theKing.isAlive) {
                    printf("\nThe Mighty King has fallen! Quickly, take his sword!\n");

                    throneRoom.isCleared = 1;

                    if (throneRoom.hasItem) {
                        pickUpItem(player, &throneRoom);
                        printf("\nYou found the King's Sword! It looks extremely powerful.\n");
                    }
                }
                return;

            case 3: // Attempt to flee
                printf("\nYou attempt to flee from the throne room!\n");
                int fleeNumber = rand() % 5 + 1;
                int playerChoice;
                printf("Pick a number (1-5): ");
                scanf("%d", &playerChoice);

                if (playerChoice == fleeNumber) {
                    printf("\nYou sprint past the King and escape safely!\n");
                    return;
                } else {
                    printf("\nThe King blocks your escape and yells, \"Coward! Face me!\"\n");
                    combat(player, &theKing, &throneRoom);

                    if (!theKing.isAlive) {
                        printf("\nThe Mighty King has fallen! Quickly, take his sword!\n");

                        throneRoom.isCleared = 1;

                        if (throneRoom.hasItem) {
                            pickUpItem(player, &throneRoom);
                            printf("\nYou found the King's Sword! It looks extremely powerful.\n");
                        }
                    }
                }
                return;

            default:
                printf("\nInvalid choice. The King glares at you menacingly.\n");
        }
    } while (1);
}


void handleRoomWithEnemies(Player *player, Room *room, Enemy *enemy) {
    if (!room || !enemy) {
        printf("\nError: Room or Enemy data not found.\n");
        return;
    }

    printf("\nYou enter the %s.\n", room->name);

    // Switch case to handle room states
    switch (room->isCleared) {
        case 0: // Room not cleared
            printf("\n%s\n", room->description);

            // Engage the enemy
            printf("\nA %s appears!\n", enemy->name);
            combat(player, enemy, room);

            // Mark the room as cleared if the enemy is defeated
            if (!enemy->isAlive) {
                room->isCleared = 1;
                printf("\nThe %s is now safe.\n", room->name);
            }
            break;

        case 1: // Room cleared
            printf("\nThe %s is quiet. The enemies here have already been defeated.\n", room->name);

            if (room->hasItem) {
                printf("\nYou notice something on the ground: %s.\n", room->item);
                printf("Do you want to pick it up? (1: Yes, 2: No): ");
                int choice;
                scanf("%d", &choice);

                if (choice == 1) {
                    pickUpItem(player, room);
                } else {
                    printf("\nYou leave the item untouched.\n");
                }
            }
            break;

        default:
            printf("\nThe room state is unknown. Please report this bug!\n");
            break;
    }
}

