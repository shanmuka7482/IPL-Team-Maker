#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Header-file for boolean data-type
typedef enum Level{
    GROUP,
    PLAYOFF,
    FINAL
}Level;
typedef struct Team {
    char team_name[50];
    int points;
    int wins;
    int loss;
    int tie;
    int matchplayed;
    bool isInPlayOffs;
    bool isInFinals;
    struct Team* next;
} Team;

typedef struct Match {
    char team_1[50];
    char team_2[50];
    int team_1_score;
    int team_2_score;
    bool isPlayOff;
    bool isFinal;
    struct Match* next;
} Match;

typedef struct Player {
    char player_name[50];
    int wickets;
    int runs;
    struct Player* next;
} Player;

//fun is used to create team
Team* addTeam(Team* head, char name[]) {
    Team* new_team = (Team*)malloc(sizeof(Team));
    strcpy(new_team->team_name, name);
    new_team->points = 0;
    new_team->loss=0;
    new_team->tie=0;
    new_team->matchplayed=0;
	new_team->wins=0;
    new_team->next = 0;
    new_team->isInPlayOffs = false;
    new_team->isInFinals = false;
    if (head == NULL) {
        return new_team;
    }

    Team* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_team;

    return head;
}


//fun used to update team points
void update_team_stats(Team* head, char name[], int points) {
    Team* current = head;
    while (current != NULL) {
        if (strcmp(current->team_name, name) == 0) {
            current->points += points;
            current->matchplayed++;
            if(points==2)
            {
            	current->wins++;
			}
			else if(points==0)
			{
				current->loss++;
			}
			else
			{
				current->tie++;
			}
            break;
        }
        current = current->next;
    }
}

void update_playoff_finals_status(Team* head, char name[], bool isInFinals) {
    Team* current = head;
    while (current != NULL) {
        if (strcmp(current->team_name, name) == 0) {
            if(isInFinals){//update finals status
                current->isInFinals=true;
            }else{//update playoff status
                current->isInPlayOffs=true;
            }
            break;
        }
        current = current->next;
    }
}

//fun is used to create Match and scores
Match* addMatch(Match* head,Team* team_head, char team_1[], char team_2[], int score1, int score2, Level level) {
    Match* newMatch = (Match*)malloc(sizeof(Match));
    strcpy(newMatch->team_1, team_1);
    strcpy(newMatch->team_2, team_2);
    newMatch->team_1_score = score1;
    newMatch->team_2_score = score2;
    if(level==PLAYOFF){
        newMatch->isPlayOff=true;
    }else if(level==FINAL){
        newMatch->isFinal=true;
    }
    newMatch->next = NULL;
    
	if(score1>score2) //team1 winner
	{
		update_team_stats(team_head, team_1, 2);
		update_team_stats(team_head, team_2, 0);
	}
	else if(score1==score2)	//Tie
	{
		update_team_stats(team_head, team_1, 1);
		update_team_stats(team_head, team_2, 1);
	}
	else	//Team2 winner
	{
		update_team_stats(team_head, team_2, 2);
		update_team_stats(team_head, team_1, 0);
	}
	
    if (head == NULL) {
        return newMatch;
    }

    Match* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newMatch;
	
    return head;
}


//fun is used to create player
Player* add_player(Player* head, char name[]) {
    Player* new_player = (Player*)malloc(sizeof(Player));
    strcpy(new_player->player_name, name);
    new_player->wickets = 0;
    new_player->runs = 0;
    new_player->next = NULL;

    if (head == NULL) {
        return new_player;
    }

    Player* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_player;

    return head;
}

//player stats update
void update_player_stats(Player* head, char name[], int wickets, int runs) {
    Player* current = head;
    while (current != NULL) {
        if (strcmp(current->player_name, name) == 0) {
            current->wickets += wickets;
            current->runs += runs;
            break;
        }
        current = current->next;
    }
}

//to get maximum wickets
char* find_plyr_with_max_wickets(Player* head) {
    if (head == NULL) {
        return NULL;
    }

    Player* current = head;
    Player* max_wicker_player = current;
    int max_wickets = current->wickets;

    while (current != NULL) {
        if (current->wickets > max_wickets) {
            max_wickets = current->wickets;
            max_wicker_player = current;
        }
        current = current->next;
    }

    return max_wicker_player->player_name;
}

//to get maximum runs
char* find_plyr_with_max_runs(Player* head) {
    if (head == NULL) {
        return NULL;
    }

    Player* current = head;
    Player* max_run_player = current;
    int max_runs = current->runs;

    while (current != NULL) {
        if (current->runs > max_runs) {
            max_runs = current->runs;
            max_run_player = current;
        }
        current = current->next;
    }

    return max_run_player->player_name;
}


//to print shedule and point table 
void schedule_and_point_table(Team* teams, Match* matches, Level level) {
    Match* current_match = matches;
    char winner_name[50];
    printf("\nMATCHES WITH SCORES:\n------------------------\n");
    while (current_match != NULL) {
    	if(current_match->team_1_score>current_match->team_2_score)
    	{
    		strcpy(winner_name,current_match->team_1);
		}
		else if(current_match->team_1_score<current_match->team_2_score)
		{
			strcpy(winner_name,current_match->team_2);
		}
		else
		{
			strcpy(winner_name,"TIE");
		}
		if(level == GROUP){
            printf("%s vs %s \n Scores : %d\t%d || Winner : %s\n\n", current_match->team_1, current_match->team_2,current_match->team_1_score,current_match->team_2_score,winner_name);
		 }else if(level==PLAYOFF){
            if(current_match->isPlayOff){
                printf("%s vs %s \n Scores : %d\t%d || Winner : %s\n\n", current_match->team_1, current_match->team_2,current_match->team_1_score,current_match->team_2_score,winner_name);
            }
        }else if(level==FINAL){
            if(current_match->isFinal){
               printf("%s vs %s \n Scores : %d\t%d || Winner : %s\n\n", current_match->team_1, current_match->team_2,current_match->team_1_score,current_match->team_2_score,winner_name);
            }
        }
        current_match = current_match->next;
    }

    printf("\nPOINTS TABLE:\n------------------------\n");
    printf("\n----------------------------------------------\n");
    printf("| %-4s | %-30s | %3s | %3s | %3s | %3s | %3s\n", "S.NO", "Team", "P", "W", "L", "T", "MPlay");
    printf("----------------------------------------------\n");
    Team* current_team = teams;
    int i=1;
    while (current_team != NULL) {
            printf("| %-4d | %-30s | %3d | %3d | %3d | %3d | %3d \n", i++, current_team->team_name,current_team->points,
                   current_team->wins,current_team->loss,current_team->tie,current_team->matchplayed);
    current_team = current_team->next;
    }
    
    printf("----------------------------------------------\n");
}

//Get random score between 150 to 200
int get_random_score(){
  int randomScore = (rand() %  (200 - 150 + 1)) + 150;
  return randomScore;
}

void findTopTeams(Team* team_header,int teamCount){
    Team* prevTopTeam = NULL;
    for(int i=1;i<=teamCount;i++){
        int pointMax=0;
        Team* current_team = team_header;
        if(teamCount==4){//PLAYOFFS
            while(current_team != NULL){
                if(pointMax <= current_team->points && !current_team->isInPlayOffs){
                    pointMax = current_team->points;
                    prevTopTeam = current_team;
                }
                current_team = current_team->next;
            }
            update_playoff_finals_status(team_header,prevTopTeam->team_name,false);
        }else if(teamCount==2){//Finals
            while(current_team != NULL){
                if(pointMax <= current_team->points && !current_team->isInFinals){
                    pointMax = current_team->points;
                    prevTopTeam = current_team;
                }
                current_team = current_team->next;
            }
            update_playoff_finals_status(team_header,prevTopTeam->team_name,true);
        }
    }
}

Match* add_scheduled_matches(Team* team_header, Match* match_header,Level level){
    Team* team_current = team_header;
    switch (level){
        case GROUP:
            while (team_current != NULL) {
        	    char curr_team_name[50];
        		strcpy(curr_team_name,team_current->team_name);
        		Team* team_remaining = team_current->next;
            		while (team_remaining != NULL){
            			if (strcmp(curr_team_name, team_remaining->team_name) != 0) {
            				    match_header=addMatch(match_header,team_header, curr_team_name, team_remaining->team_name, get_random_score(), get_random_score(), GROUP);
            			    }
            			team_remaining = team_remaining->next;
            		}
                team_current = team_current->next;
            }
            return match_header;
            break;
        case PLAYOFF:
            char temp_playoff_team_names[4][50];
            int k=0;
            while (team_current != NULL) {
                if(team_current->isInPlayOffs){
                    strcpy(temp_playoff_team_names[k],team_current->team_name);
                    k++;
                }
                team_current = team_current->next;
            }
            //Schedule PlayOff Mathes between top-1 and top-2
            match_header=addMatch(match_header,team_header, temp_playoff_team_names[0], temp_playoff_team_names[1], get_random_score(), get_random_score(), PLAYOFF);
            
            //Schedule PlayOff Mathes between top-3 and top-4
            match_header=addMatch(match_header,team_header, temp_playoff_team_names[2], temp_playoff_team_names[3], get_random_score(), get_random_score(), PLAYOFF);  
            
            return match_header;
            break;
        case FINAL:
            char temp_final_team_names[2][50];
            int ind=0;
            while (team_current != NULL) {
                if(team_current->isInFinals){
                    strcpy(temp_final_team_names[ind],team_current->team_name);
                    ind++;
                }
                team_current = team_current->next;
            }
            //Schedule Final Match between top-1 and top-2
            match_header=addMatch(match_header,team_header, temp_final_team_names[0], temp_final_team_names[1], get_random_score(), get_random_score(), FINAL);

            return match_header;
            break;
    }
    
}
int main() {
    Team* team_header = NULL;
    Match* match_header = NULL;
    Player* player_header = NULL;
    // Add teams
    team_header = addTeam(team_header, "Delhi Capitals");
    team_header = addTeam(team_header, "KingsKings XI Punjab");
    team_header = addTeam(team_header, "Chennai Super Kings");
    team_header = addTeam(team_header, "Mumbai Indians");
    team_header = addTeam(team_header, "Kolkata Knight Riders");
    team_header = addTeam(team_header, "Royal Challengers Banglore");
    team_header = addTeam(team_header,"Lucknow Super Giants");
    team_header = addTeam(team_header,"Sun Risers Hyderabad");
    team_header = addTeam(team_header,"Gujarat Titans");
    team_header = addTeam(team_header,"Rajastan Royals");
	Team* team_current = team_header;
    
    //Add scheduled group level matches
    match_header=add_scheduled_matches(team_header,match_header,GROUP);

	printf("\n\n");
    // Add players
    player_header = add_player(player_header, "Shubman Gill");
    player_header = add_player(player_header, "du Plessis");
    player_header = add_player(player_header, "Conway");
    player_header = add_player(player_header, "Kohli");
    player_header = add_player(player_header, "Mohammad Shami");
    player_header = add_player(player_header, "Rashid Khan");

    // Update player statistics
    update_player_stats(player_header, "Shubman Gill", 0, 890);
    update_player_stats(player_header, "du Plessis", 0, 730);
    update_player_stats(player_header, "Conway", 0, 672);
    update_player_stats(player_header, "Kohli", 4, 639);
	update_player_stats(player_header, "Mohammad Shami", 28, 5);
	update_player_stats(player_header, "Mohit Sharma", 27, 0);
	update_player_stats(player_header, "Rashid Khan", 27, 122);
    
    // Display Group level schedule and point table
    printf("IPL 2023 SCHEDULE\n============================================\nGROUP LEVEL MATCHES:\n============================================\n");
    schedule_and_point_table(team_header, match_header,GROUP);
    
    //PlayOffs - 4 top teams
    findTopTeams(team_header,4);
    //Add scheduled group level matches
    match_header=add_scheduled_matches(team_header,match_header,PLAYOFF);
    // Display PlayOff level schedule and point table
    printf("\n\n============================================\nPLAYOFF MATCHES:\n============================================\n");
    schedule_and_point_table(team_header, match_header,PLAYOFF);
    
    //Finals - 2 top teams
    findTopTeams(team_header,2);
    //Add scheduled group level matches
    match_header=add_scheduled_matches(team_header,match_header,FINAL);
    // Display PlayOff level schedule and point table
    printf("\n\n============================================\nFINAL MATCHE:\n============================================\n");
    schedule_and_point_table(team_header, match_header,FINAL);    

    // Get player with most wickets and runs
    char* max_wicket_player = find_plyr_with_max_wickets(player_header);
    char* max_run_player = find_plyr_with_max_runs(player_header);
    
    //purple Cap
    printf("\nPlayer with most wickets / The Purple Cap Holder : %s\n", max_wicket_player);
    
    //Orange Cap
    printf("Player with most runs / The Orange Cap Holder : %s\n", max_run_player);

}
