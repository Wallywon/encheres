/* receveur portReceveur */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#define BUFSIZE 10
int sock;
void arretParControlC(int sig) 
{
printf("terminaison par un Control-C\n");
fflush(stdout);
close(sock);
exit(-1);
}

int main (int argc, char **argv)
{
int nbclients = 3;
struct sockaddr_in adresseClients[nbclients-1];
int lgadresseClients;
int sock;
int envoye;
int recu;
struct hostent *hote[nbclients-1];
struct sockaddr_in adresseLocale;
int lgadresseLocale;
long offre=0;
char desc[500];

/* cr'eation de la socket */
if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) 
	{
  	perror("socket"); 
  	exit(1);
  	}

/*je n'arrivais pas avec un tableau d'adresse j'ai donc essayé comme ca (ca ne fonctionne pas non plus bien sur)*/
/* pr'eparation de l'adresse distante : port + la premier @ IP */
adresseClients[0].sin_family = AF_INET;
adresseClients[0].sin_port = htons("14DE014vm");
bcopy(hote[0]->h_addr, &adresseClients[0].sin_addr, hote[0]->h_length);
printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseClients[0].sin_addr));

/* pr'eparation de l'adresse distante : port + la premier @ IP */
adresseClients[1].sin_family = AF_INET;
adresseClients[1].sin_port = htons("14DE016vm");
bcopy(hote[1]->h_addr, &adresseClients[1].sin_addr, hote[1]->h_length);
printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseClients[1].sin_addr));

/* pr'eparation de l'adresse distante : port + la premier @ IP */
adresseClients[2].sin_family = AF_INET;
adresseClients[2].sin_port = htons("14DE019vm");
bcopy(hote[2]->h_addr, &adresseClients[2].sin_addr, hote[2]->h_length);
printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseClients[2].sin_addr));
/* pr'eparation de l'adresse locale : port + toutes les @ IP */
adresseLocale.sin_family = AF_INET;
adresseLocale.sin_port = htons(atoi(argv[1]));
adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);

/* recherche de l'@ IP des machines distantes */
for(int i; i<nbclients; i++){
if ((hote[i] = gethostbyname(adresseClients[i])) == NULL)
		{
  		perror("gethostbyname"); 
  		close(sock); 
  		exit(2);
  		}
}

/* attachement de la socket a` l'adresse locale */
lgadresseLocale = sizeof(adresseLocale);
if ((bind(sock,(struct sockaddr*)&adresseLocale,lgadresseLocale)) == -1) 
	{
  	perror("bind"); 
  	exit(1);
  	}
/* pr'eparation de l'adresse distante : port + la premier @ IP */
/*
for(int j=0; j < nbclients; j++) {
adresseClients[j].sin_family = AF_INET;
adresseClients[j].sin_port = htons(atoi(argv[1]));
bcopy(hote[j]->h_addr, &adresseClients[j].sin_addr, hote[j]->h_length);
printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseClients[j].sin_addr));
}

for(int j=0; j<nbclients;j++){
lgadresseClients[j] = sizeof(adresseClients[j]);
}
*/

char choix='o';
char choix1='o';
int compteur=0;
while (choix=="o" && compteur< nbClients){
	if ((recu = recvfrom(sock, &choix1,sizeof(char),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale)) != sizeof(char)) 
		{
		perror("recvfrom choix1"); 
		close(sock); 
		exit(1);
		}
if(choix1=="o"){printf("une personne est rentrée dans la vente, voulez vous continuer (il y a actuellement %d acheteurs potentiels)  \n y/n pour yes no? \n",compteur);
scanf("%c",&choix);
for(int j=0; j < nbclients; j++){
if ((envoye = sendto(sock,&choix,sizeof(char),0,(struct sockaddr*)&adresseVente,lgadressevente)) != sizeof(char)) 
	{
  	perror("sendto choix"); 
  	close(sock); 
  	exit(1);
  	}
}

}
compteur=compteur+1;

}

if(compteur==nbClients){
	choix="n";
	if ((envoye = sendto(sock,&choix,sizeof(char),0,(struct sockaddr*)&adresseVente,lgadressevente)) != sizeof(char)) 
		{
	  	perror("sendto choix"); 
	  	close(sock); 
	  	exit(1);
	  	}
	}
}

/* saisie de la description et de la mise à prix initiale */
printf("Saisir la description de la vente : ");
scanf("%s",&desc);
long offreLaPlusElevee = 0;
printf("Saisir la mise à prix initiale : ");
scanf("%d",&offre);
offreLaPlusElevee = offre;
lgadresseClients = sizeof(adresseClients[0]);
for(int j=0; j < i-1; j++) {
        envoyeDesc = sendto(sock,&desc,sizeof(char *),0,(struct sockaddr*)&adresseClients[j],lgadresseClients);
        envoyePrixInit = sendto(sock,&offre,sizeof(long),0,(struct sockaddr*)&adresseClients[j], lgadresseClients);
}
char choix2="2";
long propprix=0;
while(choix2=="o"){

if ((recu = recvfrom(sock, &propprix,sizeof(int),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale)) != sizeof(int)) 
		{
		perror("recvfrom propprix"); 
		close(sock); 
		exit(1);
		}
if(propprix>offreLaPlusElevee){
	printf("\n un acheteur vous propose %lg , voulez vous continuer l'enchere? \n");
	scanf("%c",&choix2);
		if(choix2=="n"){
			int fin=0;
			for(int j=0; j < i-1; j++) {
	        envoyeDesc = sendto(sock,&fin,sizeof(int),0,(struct sockaddr*)&adresseClients[j],lgadresseClients);}
		}
		else{
			for(int j=0; j < i-1; j++) {
	        envoyePrixInit = sendto(sock,&propprix,sizeof(long),0,(struct sockaddr*)&adresseClients[j], lgadresseClients);}
			}
		offreLaPlusElevee==propprix;
	}
else {if(propprix!=0){
	for(int j=0; j < i-1; j++) {
	envoyePrixInit = sendto(sock,&offreLaPlusElevee,sizeof(long),0,(struct sockaddr*)&adresseClients[j], lgadresseClients);}
	}
}

}

/*fin de la vente, le vendeur à tapé "n" sur le clavier"*/
/*envoie du prix gagnant à tout le monde*/
	for(int j=0; j < i-1; j++) {
	envoyePrixInit = sendto(sock,&offreLaPlusElevee,sizeof(long),0,(struct sockaddr*)&adresseClients[j], lgadresseClients);}
	}

/*seul le gagnant sait qu'il à gagné (on pourait rajouter un recevfrom qui attendrait l'adresse du gagnant)*/
close(sock);
}
