#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char full[14][14];
char wordBuff[255];
FILE * fp;
FILE * xd; 
int overlap = 0;	

void wordPick(void);
void randWordPick(void);
int validPick(int x, int y, int length, int type, int direction);

int main(void)
{
    fp = fopen("ws.html","a");	
    xd = fopen("words.txt","r");

    srand(time(NULL));
    fprintf(fp,"<html><head><style>table, th, td {border: 1px solid black;border-collapse: collapse;}th, td {padding: 5px;text-align: left;    }</style></head><body>");

    for(int i = 0; i <14; i++){		
        for(int j = 0; j < 14; j++){		
            full[j][i] = '*';	
        }
    }

    fprintf(fp,"<table style=font-family:arial;width:25%%><tr><th colspan=2>Words</th></tr><tr>");	
    wordPick();
    fprintf(fp,"<p style=\"font-size: 24pt; font-family: Courier New, Courier, monospace\">");

    for(int i = 0; i <14; i++){		
        for(int j = 0; j < 14; j++){
            if(full[j][i] == '*'){
                full[j][i] = 'A' + (rand() % 26);
            }
            fprintf(fp,"%c ",full[j][i]);
        }
        fprintf(fp,"<br/>");
    }

    fprintf(fp,"</p>");	
    fprintf(fp,"</table></body></html>");
    fclose(fp);	

    return 0;

}

void wordPick(){

    int x,y,length,type,direction,lineNum;

    for(int i = 0; i<18; i++){	

        while(1){
            lineNum = (rand() % 3000);
            xd = fopen("words.txt","r");

            for(int i = 0; i < lineNum; i++){
                fgets(wordBuff, 255, (FILE*)xd);
            }	

            if(strlen(wordBuff)-2 > 4 && strlen(wordBuff)-2 < 10){
                break;
            }
        }

        while(1){
            length = strlen(wordBuff)-2;
            x = (rand() % 14);
            y = (rand() % 14);	
            type = (rand() % 3);	
            direction = (rand() % 2);		

            if(validPick(x,y,length,type,direction) == 1){
                //overlap = 0;
                break;
            }else{
                continue;
            }			
        }

        for(int j = 0; j <= length; j++){

            if(type == 0){
                if(direction == 0){
                    full[x+j][y] = wordBuff[j];
                }
                if(direction == 1){
                    full[x-j][y] = wordBuff[j];
                }
            }

            if(type == 1){
                if(direction == 0){
                    full[x][y+j] = wordBuff[j];
                }
                if(direction == 1){
                    full[x][y-j] = wordBuff[j];
                }

            }

            if(type == 2){
                if(direction == 0){
                    full[x+j][y+j] = wordBuff[j];
                }
                if(direction == 1){
                    full[x-j][y-j] = wordBuff[j];
                }
            }
        }

        if(i%2 == 0 && i!=0){
            fprintf(fp,"</tr><tr><td>%s</td>",wordBuff);
        }else{
            fprintf(fp,"<td>%s</td>",wordBuff);
        }

    }
}

void randWordPick(){	

    int x,y,length,type,direction;

    for(int i = 0; i<14; i++){

        while(1){
            x = (rand() % 14);
            y = (rand() % 14);
            length = (4 + (rand() % 4));
            type = (rand() % 3);
            direction = (rand() % 2);

            if(validPick(x,y,length,type,direction) == 1){
                break;
            }else{
                continue;
            }			
        }

        char tempWord[length+1];		

        for(int j = 0; j < length; j++){

            if(type == 0){		
                if(direction == 0){
                    tempWord[j] = full[x+j][y];
                }
                if(direction == 1){
                    tempWord[j] = full[x-j][y];
                }				
            }

            if(type == 1){				
                if(direction == 0){
                    tempWord[j] = full[x][y+j];
                }
                if(direction == 1){
                    tempWord[j] = full[x][y-j];
                }				
            }

            if(type == 2){				
                if(direction == 0){
                    tempWord[j] = full[x+j][y+j];
                }
                if(direction == 1){
                    tempWord[j] = full[x-j][y-j];
                }
            }

        }

        tempWord[length+1] = '\0';

        if(i%2 == 0 && i!=0){
            fprintf(fp,"</tr><tr><td>%s</td>",tempWord);
        }else{
            fprintf(fp,"<td>%s</td>",tempWord);
        }

    }
}

int validPick(int x,int y,int length, int type, int direction){

    if(type == 0){

        if(direction == 0){
            if((x+length) < 14){
                for(int i = 0; i <= length; i++){
                    if(wordBuff[i] != full[x+i][y] && full[x+i][y] != '*'){
                        overlap = 1;	
                        return 0;
                    } 				
                }
                return 1;
            }else{
                return 0;
                overlap = 0;
            }			
        }

        if(direction == 1){
            if((x-length) > -1){
                for(int i = 0; i <= length; i++){
                    if(wordBuff[i] != full[x-i][y] && full[x-i][y] != '*'){
                        overlap = 1;
                        return 0;
                    } 				
                }
                return 1;
            }else{
                return 0;
                overlap = 0;
            }
        }

    }
    //---------------------//
    if(type == 1){

        if(direction == 0){
            if((y+length) <= 14){
                for(int i = 0; i <= length; i++){
                    if(wordBuff[i] != full[x][y+i] && full[x][y+i] != '*'){
                        overlap = 1;	
                        return 0;
                    } 				
                }
                return 1;
            }else{
                return 0;
                overlap = 0;
            }			
        }

        if(direction == 1){
            if((y-length) > -1){
                for(int i = 0; i <= length; i++){
                    if(wordBuff[i] != full[x][y-i] && full[x][y-i] != '*'){
                        overlap = 1;			
                        return 0;
                    } 				
                }
                return 1;
            }else{
                return 0;
                overlap = 0;
            }
        }

    }
    //---------------------//
    if(type == 2){

        if(direction == 0){
            if((x+length) <= 14 && (y+length) <= 14){
                for(int i = 0; i <=length; i++){
                    if(wordBuff[i] != full[x+i][y+i] && full[x+i][y+i] != '*'){
                        overlap = 1;
                        return 0;
                    } 				
                }
                return 1;
            }else{
                return 0;
                overlap = 0;
            }	
        }

        if(direction == 1){
            if((x-length) > -1 && (y-length) > -1){
                for(int i = 0; i <= length; i++){
                    if(wordBuff[i] != full[x-i][y-i] && full[x-i][y-i] != '*'){
                        overlap = 1;
                        return 0;
                    } 				
                }
                return 1;
            }else{
                return 0;
                overlap = 0;
            }
        }
    }
    fprintf(stderr, "Invalid choice\n");
    return 0;
}
