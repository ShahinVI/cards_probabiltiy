/*
this code is made to count the number of occurance of different types of pairs
it creates a deck of cards, shuffle it, then deals a hand
after that it counts the occurance of the pairs found
in the end the result is a representation of the types of hands occured and the percentage of it occuring
you can adjust number_of_hands to see different result of probability
created by Aiman Shahin
12.04.2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // used to randomly shuffle the deck of cards
#include <string.h> // used for string manupliation
#include <stdbool.h> // used for boolean

#define DECK_SIZE 52                // number of cards in deck
#define HAND_SIZE 7                 // hand size can be modified
#define NUMBER_OF_PAIRS 6           // the pairs we are looking for
#define NUMBER_OF_HANDS 1000000     // number of time we are reshuffling the deck and giving a new hand

// suits, pips and pairs enums
typedef enum suits { CLUBS, DIAMONDS, HEARTS, SPADES} suit;
typedef enum pips {Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King} pip;
typedef enum pairs {No_Pair,One_Pair,Two_Pair,Three_Of_a_Kind,Full_House,Four_Of_a_Kind} pair;

// structure of the cards, somehow the suits is not really relevant for this code
typedef struct playingCards{
    suit s;
    pip p;
}card;

// after creating the deck of cards or assiging the hand we can print any size of cards we have
// this function is used for debugging
void printDeck(card * Cards, int lenght){
    char p[20]; // character to hold number of card pip
    char s[20]; // character to hold type of card suit
    for(int i=0;i<lenght;i++){
        switch(Cards[i].s){
            case 0: strcpy(s,"CLUBS");break;
            case 1: strcpy(s,"DIAMONDS");break;
            case 2: strcpy(s,"HEARTS");break;
            case 3: strcpy(s,"SPADES");break;
            default:printf("something went wrong\n");break;
        }
        switch(Cards[i].p){
            case 0: strcpy(p,"ACE");break;
            case 1: strcpy(p,"Two");break;
            case 2: strcpy(p,"Three");break;
            case 3: strcpy(p,"Four");break;
            case 4: strcpy(p,"Five");break;
            case 5: strcpy(p,"Six");break;
            case 6: strcpy(p,"Seven");break;
            case 7: strcpy(p,"Eight");break;
            case 8: strcpy(p,"Nine");break;
            case 9: strcpy(p,"Ten");break;
            case 10: strcpy(p,"Jack");break;
            case 11: strcpy(p,"Queen");break;
            case 12: strcpy(p,"King");break;
            default:printf("something went wrong\n");break;
        }
        printf("%s of %s\n",p,s);
    }
    printf("\n\n");
}

// filling the struct deckOfCards with cards, creating the deck of cards
void createDeck(card *deckOfCards){
    int i=0;
    for(int p=0;p<13;p++){
        for(int s=0;s<4;s++){
            deckOfCards[i].p=p;
            deckOfCards[i].s=s;
            i++;    
        }
    }
}

// shuffle the deck of card
void shuffle(card * deckOfCards, int lengthOfDeck){
    srand(time(NULL));
    int swap_index;
    for(int l=0;l<lengthOfDeck;l++){
        swap_index= rand()%(lengthOfDeck);
        card temp = deckOfCards[l];
        deckOfCards[l]= deckOfCards[swap_index];
        deckOfCards[swap_index]= temp;
    }
}

// deal card to the hand
void dealCards(card * hand,card * deckOfCards, int handSize){
    for(int j=0;j<handSize;j++){
        hand[j]=deckOfCards[j];
    }
}


// in this function we are counting the number of elements repeated. we will only focuse on the pip
// since the pairs we are searching for doesnt require to look into suits
int * countOccuarance(card * hand, int handSize, int * sizeRepetitionArray){
    int * repetition = malloc(handSize * sizeof(int));
    
    for(int i=0;i<handSize;i++){
        repetition[i]=1;
        *sizeRepetitionArray=*sizeRepetitionArray+1;
        
        for(int j=i+1;j<handSize;j++){
            // if pairs found
            if(hand[i].p==hand[j].p){
                repetition[i]=repetition[i]+1; //increment the pair by 1, indicating that the same element occured
                
                // in this part to save time we will push the evaluated element to the end of the array
                card temp=hand[j];
                for(int z=j+1;z<handSize;z++){
                    hand[z-1]=hand[z];
                }
                hand[handSize-1]=temp;
                // decrease the hand size to avoid redundancy and error
                handSize--;
                // since the array was shifted to the left we need to remain at the same position
                j--;
            }
        }
    }
    return repetition;
}

// in this part of the code we count the number of pairs occured and which type it is
void countPairs(int * repetition,int sizeRepetitionArray, int * pairsFound){
    // booleans to correctly evaluate the hand and avoid redundant cards
    bool threeOfaKind = false;
    bool onePair = false;
    bool twoPair = false;
    bool pairOccured = false;
    // looping through the repetition array
    for(int i=0;i<sizeRepetitionArray;i++){
        switch(repetition[i]){
            case 2: if(onePair){ // if one Pair was already found
                        pairsFound[One_Pair]--; //decrement one pair
                        pairsFound[Two_Pair]++; // increment two pair
                        onePair=false;          // make sure to make onePair flag false to avoid error while counting
                    }else{
                        pairsFound[One_Pair]++; // if there is no other one pairs increment one pair if found
                        onePair=true;           // set the one pair flag high in case another one occured then we increment two pairs and decrement one pair
                        }
                        pairOccured=true;       // we raise this flag to inform that a pair occured and not increment no pairs
                        break;
            case 3: pairsFound[Three_Of_a_Kind]++;  // if 3 of a kind was found increment 3 of a kind once
                    threeOfaKind=true;              // this flag is raised so when testing for full house we make sure that both three of a kind and a pair was found
                    pairOccured=true;               // we raise this flag to inform that a pair occured and not increment no pairs
                    break;
            case 4: pairsFound[Four_Of_a_Kind]++;   // increment 4 of a kind if found
                    pairOccured=true;               // we raise this flag to inform that a pair occured and not increment no pairs
                    break;
            default: break;
        }
    }
    // if a pair was found and a three of a kind was found then decrement them and increment a full house
    if(onePair&&threeOfaKind){
        pairsFound[One_Pair]--;
        pairsFound[Three_Of_a_Kind]--;
        pairsFound[Full_House]++;
    }
    // if no pair occured increment no pair
    if(!pairOccured){
        pairsFound[No_Pair]++;
    }
}

// printing the pairs found and their percentages
void printPairs(int * pairsFound){
    
    int sum = 0;
    for(int i=0;i<NUMBER_OF_PAIRS;i++){
        sum = sum+pairsFound[i];
    }

    for(int i=0;i<NUMBER_OF_PAIRS;i++){
        switch(i){
            case No_Pair: printf("No pairs were found \t\t%d\t times with a total percentage of \t%.2lf \n",pairsFound[i],((double)pairsFound[i]/(double)sum)*100.0);break;
            case One_Pair: printf("One pairs were found \t\t%d\t times with a total percentage of \t%.2lf \n",pairsFound[i],((double)pairsFound[i]/(double)sum)*100.0);break;
            case Two_Pair: printf("Two pairs were found \t\t%d\t times with a total percentage of \t%.2lf \n",pairsFound[i],((double)pairsFound[i]/(double)sum)*100.0);break;
            case Three_Of_a_Kind: printf("Three of a kind were found \t%d\t times with a total percentage of \t%.2lf \n",pairsFound[i],((double)pairsFound[i]/(double)sum)*100.0);break;
            case Full_House: printf("Full_House were found \t\t%d\t times with a total percentage of \t%.2lf \n",pairsFound[i],((double)pairsFound[i]/(double)sum)*100.0);break;
            case Four_Of_a_Kind: printf("No pairs were found \t\t%d\t times with a total percentage of \t%.2lf \n",pairsFound[i],((double)pairsFound[i]/(double)sum)*100.0);break;
        }
    }
}

int main(){
    // initialize variables
    int pairsFound[NUMBER_OF_PAIRS] = {0,0,0,0,0,0};    // used to record all pairs occured
    card deckOfCards[DECK_SIZE];                        // array of struct cards to hold the deck of cards
    card hand[HAND_SIZE];                               // hand of player
    int *repetition;                                    // this holds the number of occurance of elements
    int sizeRepetitionArray;                            // this variable shows the number of repetition occurance occured


    createDeck(deckOfCards);                            // creation of the deck

    // a loop of shuffling, dealing of cards and counting pairs a number of hands times
    for(int i = 0;i<NUMBER_OF_HANDS;i++){
        //printDeck(deckOfCards,DECK_SIZE);             // used for debug
        shuffle(deckOfCards,DECK_SIZE);
        //printDeck(deckOfCards,DECK_SIZE);             // used for debug
        dealCards(hand,deckOfCards,HAND_SIZE);
        //printDeck(hand,HAND_SIZE);                    // used for debug
        
        sizeRepetitionArray=0;
        repetition= countOccuarance(hand,HAND_SIZE,&sizeRepetitionArray);
        
        // used for debug
        //for(int i=0;i<sizeRepetitionArray;i++){
        //    printf("%d  ,",repetition[i]);
        //}
        //printf("\n");
        
        countPairs(repetition,sizeRepetitionArray,pairsFound);
    }
    printPairs(pairsFound);

}