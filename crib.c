#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#define TOPX 15

void chooseDiscard(void);
int promptForHand(char hand[]);
int evalFive(int hand[]);
void rankFour(char hand[], int handVal[], double bestHands[TOPX][5], double unseen);


int main(void){
	while(1){
		chooseDiscard();
		printf("\n\n\n");
	}
	return 0;
}

void chooseDiscard(void){
	char hand[7];
	int numOfCards, i, j, k, l, handVal[5];
	double bestHands[TOPX][5]={0};

	numOfCards=promptForHand(hand);
	if(numOfCards==6){
		for(i=0;i<5;i++){
			for(j=i+1;j<6;j++){
				for(k=0,l=0;k<6;k++){
					if(k!=i && k!=j){
						handVal[l]=hand[k]; //implicit casting
						l++;
					}
				}
				rankFour(hand,handVal,bestHands,46);
			}
		}
	}
	else{
		for(i=0;i<5;i++){
			for(k=0,l=0;k<5;k++){
				if(k!=i){
					handVal[l]=hand[k];
					l++;
				}
			}
			rankFour(hand,handVal,bestHands,47);
		}
	}
	printf("\nHand:   Points:\n");
	for(i=0;i<TOPX;i++){
		if(!bestHands[i][0])
			break;
		for(j=0;j<4;j++){
			if(bestHands[i][j]>=2 && bestHands[i][j]<=9)
				printf("%.0f",bestHands[i][j]);
			else if(bestHands[i][j]==1)
				printf("A");
			else if(bestHands[i][j]==13)
				printf("K");
			else if(bestHands[i][j]==12)
				printf("Q");
			else if(bestHands[i][j]==11)
				printf("J");
			else
				printf("T");
		}
		printf("%9.2f\n",bestHands[i][4]);
	}
	if(numOfCards==5){
		for(i=0;i<5;i++)
			handVal[i]=hand[i];
		printf("\nCurrently, there are %i points in your hand.\n",evalFive(handVal));
	}
}

//returns number of cards dealt (done)
int promptForHand(char hand[]){
	int i;
	printf("Enter the hand you were dealt.\nIf you were dealt a King, two Eights, a Jack, an Ace, and a Ten,\nenter 'K88JAT' as your hand.\nYou must manually add points for flushes.\n\n");
	scanf("%s",hand);
	for(i=0;hand[i]!='\0';i++){
		if(hand[i]>='2' && hand[i]<='9')
			hand[i]-=48;
		else if(hand[i]=='A' || hand[i]=='a')
			hand[i]=1;
		else if(hand[i]=='K' || hand[i]=='k')
			hand[i]=13;
		else if(hand[i]=='Q' || hand[i]=='q')
			hand[i]=12;
		else if(hand[i]=='J' || hand[i]=='j')
			hand[i]=11;
		else
			hand[i]=10;
	}
	return i;
}

//returns points from 5 card hand (done)
int evalFive(int hand[]){
	int points=0, i, j, count, run[5], fakeHand[5]; //{length,lastcard,maxlength,runfactor,maxrunfactor}

	//count pairs
	for(i=0;i<4;i++){
		for(j=i+1;j<5;j++){
			if(hand[j]==hand[i])
				points+=2;
		}
	}

	//count runs
	run[2]=0;
	run[4]=0;
	for(i=0;i<5;i++){
		run[0]=1;
		run[1]=hand[i];
		run[3]=1;
		while(1){
			count=0;
			for(j=0;j<5;j++){
				if(hand[j]==run[1])
					count++;
			}
			run[3]*=count;
			for(j=0;j<5;j++){
				if(hand[j]==run[1]+1){
					run[0]++;
					run[1]++;
					break;
				}
			}
			if(j==5)
				break;
		}
		if(run[0]>2 && run[0]>run[2]){
			run[2]=run[0];
			run[4]=run[3];
		}
	}
	points+=(run[2]*run[4]);

	//count 15s last
	for(i=0;i<5;i++){
		if(hand[i]>10)
			fakeHand[i]=10;
		else
			fakeHand[i]=hand[i];
	}
	for(i=3;i<32;i++){
		count=0;
		if(i%2)
			count+=fakeHand[0];
		if((i/2)%2)
			count+=fakeHand[1];
		if((i/4)%2)
			count+=fakeHand[2];
		if((i/8)%2)
			count+=fakeHand[3];
		if(i>16)
			count+=fakeHand[4];
		if(count==15)
			points+=2;
	}

	return points;
}

//determines where handVal fits in best hands (done)
void rankFour(char hand[], int handVal[], double bestHands[TOPX][5], double unseen){
	int pointsForFour=0, countKs, k, l;
	double fourRank;

	for(k=1;k<=13;k++){
		for(countKs=0,l=0;l<6;l++){
			if(hand[l]==k)
				countKs++;
		}
		handVal[4]=k;
		pointsForFour+=(evalFive(handVal)*(4-countKs));
	}
	fourRank=pointsForFour/unseen;
	for(k=0;k<4;k++){
		if(handVal[k]==11)
			fourRank+=12.0/51;
	}
	for(k=0;k<TOPX;k++){
		if(fourRank>=bestHands[k][4]){
			for(l=TOPX-1;l>k;l--){
				bestHands[l][0]=bestHands[l-1][0];
				bestHands[l][1]=bestHands[l-1][1];
				bestHands[l][2]=bestHands[l-1][2];
				bestHands[l][3]=bestHands[l-1][3];
				bestHands[l][4]=bestHands[l-1][4];
			}
			bestHands[l][0]=handVal[0];
			bestHands[l][1]=handVal[1];
			bestHands[l][2]=handVal[2];
			bestHands[l][3]=handVal[3];
			bestHands[l][4]=fourRank;
			break;
		}
	}
}
