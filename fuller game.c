#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIALOGUE_LENGTH 500
#define MAX_NAME_LENGTH 16
#define MAX_DESCRIPTION_LENGTH 200
#define MAX_INVENTORY_SIZE 30

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



void intro(Player *player);
void Tavern(Player *player);
void explorePath(Player *player);
void riverCrossing(Player *player);
void caveEncounter(Player *player);
void farmersField(Player *player);
void combat(Player *player, Enemy *enemy);
void printInventory(Player *player);
void pickUpItem(Player *player, Room *room);
void unlockRoom(Player *player, Room *room);
void shop(Player *player);
void interactWithNPC(Player *player, struct NPC *npc);
void townSquare(Player *player);

int main() {
    Player player;
    intro(&player);
    
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
    player->name[MAX_NAME_LENGTH] = '\0';  // Ensure null termination
    
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
        printf("\nThe people around you are too drunk to talk coherently. You're not getting anywhere.\n\n");
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

void explorePath(Player *player) {
    int choice;

    printf("\nYou get to the fork in the road.\n\n");
    printf("1. Take the North path, which looks dark and narrow.\n\n");
    printf("2. Take the East path, where you can hear some running water.\n\n");
    printf("3. Take the South path, which looks well worn.\n\n");
    printf("What do you choose? ");
    scanf("%d", &choice);

    if (choice == 2) {
        riverCrossing(player);
    } else if (choice == 1) {
        caveEncounter(player);
    } else if (choice == 3) {
        farmersField(player);
    } else {
        printf("\nInvalid choice. Try again.\n\n");
    }
}

void riverCrossing(Player *player) {
    int choice;

    printf("\nYou reach a fast-flowing river.\n\n");
    printf("1. Try to swim across.\n\n");
    printf("2. Look for a bridge or some rocks to cross on.\n\n");
    printf("What do you choose? ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nThe river is too strong! You struggle to stay afloat.\n\n");
        player->health -= 20;
        printf("You lose 20 health. Current health: %d\n\n", player->health);
    } else if (choice == 2) {
        printf("\nYou find a fallen tree bridging the river and carefully cross over.\n\n");
    } else {
        printf("\nInvalid choice. You lose your way and wander back to the forest entrance.\n\n");
    }

    explorePath(player);
}

void caveEncounter(Player *player) {
    Enemy goblin = {"Goblin", 30, 1};
    Room cave = {
        .name = "Ancient Cave",
        .description = "a weathered stone door in the shape of a circle. Looks like it has'nt been open in ages",
        .item = "Ancient Amulet",
        .hasItem = 1,
        .locked = 1
    };
    
   struct NPC soldier = {
    .name = "Wounded Knight",
    .dialogue = "I was part of an elite squad tasked with defeating the Lich King in the graveyard across that river. My companions... they didn't make it. Will you do what we could'nt, and kill the lich?",
    .hasQuest = 1
};

    int choice;
    printf("\nYou arrive at the cave entrance. The air grows colder here, and a small campfire casts shadows on the ancient stone walls. Beside it sits a wounded knight, his armor bearing the crest of the Royal Guard.\n\n");

    do {
        printf("What would you like to do?\n\n");
        printf("1. Examine the cave entrance\n");
        printf("2. Speak with the wounded knight\n");
        printf("3. Open inventory\n");
        printf("4. Return to the previous path\n\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nYou approach the entrance, you see %s\n\n", cave.description);
                if (cave.locked) {
                    printf("The door is sealed by a overgrown lock. Perhaps a special key would help?\n\n");
                    unlockRoom(player, &cave);
                    if (!cave.locked) {
                        printf("The lock falls off as the door slowly opens, revealing a dark passage beyond.\n\n");
                        printf("Do you wish to enter? (1 for Yes, 2 for No): ");
                        int enterChoice;
                        scanf("%d", &enterChoice);
                        if (enterChoice == 1) {
                            printf("\nYou step into the darkness...\n\n");
                            combat(player, &goblin);
                            if (cave.hasItem) {
                                pickUpItem(player, &cave);
                            }
                            
                            // After defeating the goblin and potentially getting the item
                            printf("\nDeeper in the cave, you find yourself in a dimly lit chamber.\n");
                            printf("What would you like to do?\n\n");
                            printf("1. Continue forward towards the bridge\n");
                            printf("2. Return to the cave entrance\n\n");
                            
                            int deeperChoice;
                            scanf("%d", &deeperChoice);
                            
                            if (deeperChoice == 1) {
                                bridgeEncounter(player);
                                return;
                            }
                        }
                    }
                }
                break;

            case 2:
            printf("You walk over to the campfire, it seems like he does'nt notice you\n");
                interactWithNPC(player, &soldier);
                printf("'Be warned, traveler. The Lich's power grows stronger with each soul he claims.'\n\n");
                break;

            case 3:
                printInventory(player);
                break;

            case 4:
                printf("\nYou decide to return to the forest path.\n\n");
                explorePath(player);
                return;

            default:
                printf("\nInvalid choice. Please choose again.\n\n");
        }
    } while (1);
}

void bridgeEncounter(Player *player) {
    Enemy bridgeTroll = {"Bridge Troll", 50, 1};
    int choice;

    printf("\nAs you venture deeper into the cave, you come upon a vast chasm.\n");
    printf("A narrow stone bridge spans the darkness, its ancient stonework crumbling at the edges.\n");
    printf("The depths below are shrouded in impenetrable darkness, and a cold wind howls through the cavern.\n\n");
    printf("As you step onto the bridge, a massive figure looms ahead of you. It's a Bridge Troll!\n");
    printf("The troll growls, \"Pay the toll or face my wrath!\".\n\n");

    do {
        printf("What would you like to do?\n");
        printf("1. Fight the Bridge Troll\n");
        printf("2. Attempt to negotiate\n");
        printf("3. Try to flee\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nYou prepare to fight the troll on the narrow bridge!\n");
                combat(player, &bridgeTroll);
                if (!bridgeTroll.isAlive) {
                    printf("\nThe troll lets out a final roar before collapsing. You carefully step over its massive body and continue forward.\n\n");
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
                        printf("Remaining gold: %d\n\n", player->gold);
                        return;
                    } else {
                        printf("\nThe troll snarls, \"Then you shall face my wrath!\"\n");
                        combat(player, &bridgeTroll);
                        if (!bridgeTroll.isAlive) {
                            printf("\nThe troll lets out a final roar before collapsing. You carefully step over its massive body and continue forward.\n\n");
                        }
                        return;
                    }
                } else {
                    printf("\nThe troll laughs at your empty pockets and attacks you!\n");
                    combat(player, &bridgeTroll);
                    if (!bridgeTroll.isAlive) {
                        printf("\nThe troll lets out a final roar before collapsing. You carefully step over its massive body and continue forward.\n\n");
                    }
                    return;
                }

            case 3:
                printf("\nYou attempt to flee across the bridge!\n");
                printf("Pick a number (1-3): ");
                int fleeNumber, correctFleeNumber;
                scanf("%d", &fleeNumber);
                correctFleeNumber = rand() % 3 + 1;
                if (fleeNumber == correctFleeNumber) {
                    printf("\nYou sprint past the troll and make it across the bridge safely!\n");
                    return;
                } else {
                    printf("\nThe troll blocks your escape and roars, \"Coward! Face me!\"\n");
                    combat(player, &bridgeTroll);
                    if (!bridgeTroll.isAlive) {
                        printf("\nThe troll lets out a final roar before collapsing. You carefully step over its massive body and continue forward.\n\n");
                    }
                    return;
                }

            default:
                printf("\nInvalid choice. The troll glares at you menacingly.\n");
        }
    } while (1);
}

void oldLadyBridge(Player *player) {
    int choice;
    struct NPC oldLady = {
        .name = "Old Lady",
        .dialogue = "Ah, traveler, you seem brave enough to face the dangers of this land. Beware, for beyond this bridge lies the lair of a fearsome dragon.",
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
                printf("\nShe continues, \"The dragon's lair is beyond the mountains. Many have tried to defeat it, but none have returned.\"\n");
                printf("\"They say its scales are impenetrable, but perhaps there's a way to outsmart it.\"\n");
                printf("She smiles faintly and adds, \"Good luck, brave traveler.\"\n\n");
                break;

            case 2:
                printf("\nYou decide to cross the bridge silently, nodding politely to the old lady as you pass.\n");
                printf("Her voice follows you: \"Remember, not all battles are won by strength alone.\"\n");
                printf("\nYou make it across the bridge safely.\n\n");
                return;

            case 3:
                printf("\nYou decide to turn back, feeling unprepared for what lies ahead.\n");
                return;

            default:
                printf("\nInvalid choice. Please choose again.\n");
        }
    } while (1);
}

void combat(Player *player, Enemy *enemy) {
    if (!enemy->isAlive) {
        printf("\nThe %s is already dead.\n\n", enemy->name);
        return;
    }
    
    printf("\nA %s lunges from the shadows! It has %d health.\n\n", enemy->name, enemy->health);
    int escapeNumber, dodgeNumber, playerChoice;

    while (enemy->isAlive && player->health > 0) {
        printf("\nCombat Options:\n");
        printf("1. Attack\n");
        printf("2. Open Inventory\n");
        printf("3. Block\n");
        printf("4. Attempt Escape\n\n");
        printf("What will you do? ");
        scanf("%d", &playerChoice);

        switch (playerChoice) {
            case 1: // Attack
                printf("\nYou strike at the %s!\n", enemy->name);
                enemy->health -= 20;
                printf("The %s has %d health remaining!\n\n", enemy->name, enemy->health);
                
                if (enemy->health <= 0) {
                    printf("You have slain the %s!\n\n", enemy->name);
                    enemy->isAlive = 0;
                    return;
                }
                break;

            case 2: // Inventory
                while (1) {
                    printInventory(player);
                    if (player->itemCount > 0) {
                        printf("\nEnter item number to use (0 to return to combat): ");
                        int itemChoice;
                        scanf("%d", &itemChoice);
                        
                        if (itemChoice == 0) break;
                        
                        if (itemChoice > 0 && itemChoice <= player->itemCount) {
                            itemChoice--; // Adjust for 0-based array
                            
                            // Item usage logic
                            if (strcmp(player->inventory[itemChoice], "Health Potion") == 0) {
                                player->health += 20;
                                printf("\nYou drink the Health Potion. Health restored to %d!\n", player->health);
                                // Remove used item
                                for (int i = itemChoice; i < player->itemCount - 1; i++) {
                                    strcpy(player->inventory[i], player->inventory[i + 1]);
                                }
                                player->itemCount--;
                                break;
                            }
                            else if (strcmp(player->inventory[itemChoice], "Damage Potion") == 0) {
                                enemy->health -= 50;
                                printf("\nYou throw the Damage Potion! The %s takes 50 damage!\n", enemy->name);
                                if (enemy->health <= 0) {
                                    printf("The %s has been defeated!\n\n", enemy->name);
                                    enemy->isAlive = 0;
                                    return;
                                }
                                // Remove used item
                                for (int i = itemChoice; i < player->itemCount - 1; i++) {
                                    strcpy(player->inventory[i], player->inventory[i + 1]);
                                }
                                player->itemCount--;
                                break;
                            }
                            else if (strcmp(player->inventory[itemChoice], "Apple") == 0) {
                                player->health += 5;
                                printf("\nYou quickly eat the Apple. Health restored to %d!\n", player->health);
                                // Remove used item
                                for (int i = itemChoice; i < player->itemCount - 1; i++) {
                                    strcpy(player->inventory[i], player->inventory[i + 1]);
                                }
                                player->itemCount--;
                                break;
                            }
                        }
                    } else {
                        printf("\nPress 0 to return to combat: ");
                        int temp;
                        scanf("%d", &temp);
                        if (temp == 0) break;
                    }
                }
                continue; // Skip enemy turn after inventory management

            case 3: // Block
                printf("\nYou raise your guard, ready to block the next attack!\n");
                break;

            case 4: // Escape
                printf("\nYou attempt to escape!\n");
                printf("Pick a number (1-3): ");
                scanf("%d", &escapeNumber);
                
                int correctEscapeNumber = rand() % 3 + 1;
                
                if (escapeNumber == correctEscapeNumber) {
                    printf("\nYou successfully escape from the battle!\n\n");
                    return;
                } else {
                    printf("\nEscape failed! The %s blocks your path!\n\n", enemy->name);
                }
                break;

            default:
                printf("\nInvalid choice! The %s takes advantage of your hesitation!\n", enemy->name);
                break;
        }

        // Enemy turn
        if (enemy->isAlive) {
            printf("\nThe %s prepares to attack!\n", enemy->name);
            printf("Try to dodge! Pick a number (1-2): ");
            scanf("%d", &dodgeNumber);
            
            int correctDodgeNumber = rand() % 2 + 1;
            
            if (dodgeNumber == correctDodgeNumber) {
                printf("\nYou successfully dodge the attack!\n\n");
            } else {
                int damage = (playerChoice == 3) ? 0 : 15; // No damage if blocked
                if (playerChoice == 3) {
                    printf("\nYou successfully block the attack!\n\n");
                } else {
                    player->health -= damage;
                    printf("\nThe attack hits! You take %d damage!\n", damage);
                    printf("You have %d health remaining.\n\n", player->health);
                }
            }
        }

        if (player->health <= 0) {
            printf("\nYou have been defeated by the %s. Game Over.\n\n", enemy->name);
            exit(0);
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
        if (player->itemCount < MAX_INVENTORY_SIZE) {  // Use MAX_INVENTORY_SIZE instead of MAX_INVENTORY
            strcpy(player->inventory[player->itemCount], room->item);
            player->itemCount++;
            printf("\nYou picked up %s\n", room->item);
            room->hasItem = 0;
            room->item[0] = '\0';  // Clear the item
        } else {
            printf("\nYour inventory is full!\n");
        }
    } else {
        printf("\nThere is nothing to pick up here.\n");
    }
}

void interactWithNPC(Player *player, struct NPC *npc) {
    printf("\n%s says: %s\n", npc->name, npc->dialogue);
    
    if (npc->hasQuest) {
        if (!npc->quest.isActive && !npc->quest.isCompleted) {
            printf("\nWill you accept this quest? (1: Yes, 2: No): ");
            int choice;
            scanf("%d", &choice);
            
            if (choice == 1) {
                // Initialize quest details only after player accepts
                npc->quest = (struct Quest){
                    .isActive = 1,
                    .isCompleted = 0,
                    .rewardGold = 150,
                };
                strcpy(npc->quest.targetItem, "Lich's Head");
                strcpy(npc->quest.description, "Defeat the Lich King and bring back his head as proof");
                
                printf("\nQuest accepted: %s\n", npc->quest.description);
                printf("Reward: %d gold\n", npc->quest.rewardGold);
            }
        }
        else if (npc->quest.isActive && !npc->quest.isCompleted) {
            // Check if player has the Lich's Head
            for (int i = 0; i < player->itemCount; i++) {
                if (strcmp(player->inventory[i], "Lich's Head") == 0) {
                    printf("\nYou present the Lich's Head to the wounded knight.\n");
                    printf("'You... you actually did it! The realm is safer thanks to your bravery.'\n");
                    
                    // Remove the Lich's Head from inventory
                    for (int j = i; j < player->itemCount - 1; j++) {
                        strcpy(player->inventory[j], player->inventory[j + 1]);
                    }
                    player->itemCount--;
                    
                    // Give reward
                    player->gold += npc->quest.rewardGold;
                    printf("\nReceived %d gold!\n", npc->quest.rewardGold);
                    printf("Current gold: %d\n", player->gold);
                    
                    npc->quest.isCompleted = 1;
                    npc->quest.isActive = 0;
                    strcpy(npc->dialogue, "Thank you for defeating the Lich King. The realm is in your debt.");
                    return;
                }
            }
            printf("\n'Remember, bring me the Lich's Head as proof of your victory.'\n");
        }
        else if (npc->quest.isCompleted) {
            printf("\nThe knight nods at you with respect.\n");
        }
    }
}

void farmersField(Player *player) {
    int choice;
    
    printf("\nYou are standing in front of farm.\n");
    printf("An old man is trying to get your attention.\n");
    printf("What would you like to do.\n");
    printf("1. Walk over and listen to what he has to say.\n");
    printf("2. continue on your journey and ignore the man in need.\n");
    printf("What do you choose? \n");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("He says \"Hello traveler I need your help a troll has stolen the key I was indebted to by a knight.\"\n");
        
    } else if (choice == 2) {
        printf("You find a bridge across the river however it has a strange aura.\n");
    /* we need to add the troll encounter here */
    } 
    else {
        printf("Invalid choice. You lose your way and wander back to the forest entrance.\n");
    }

        
  explorePath(player);
    
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
    }
}