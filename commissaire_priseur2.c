/* serveur commissaire priseur première version*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


int main(int argc, char **argv)
{
int sock;
int envoyeDesc, envoyeVoid, envoyePrixInit, envoyeNouvelleOffre, envoyeDerniereOffre;
int recu;

int nbclients = 3;
int nbacheteurs = 0;

struct sockaddr_in adresseClients[nbclients-1];
int lgadresseClients[nbclients-1];

struct sockaddr_in adresseLocale;
int lgadresseLocale;

struct hostent *hote[nbclients-1];
char chaine[25];

adresseClients["14DE014vm","14DE016vm","14DE019vm"];

char choix='o';

long offreLaPlusElevee = 0;
long offre;

char desc[200];
int cpt = 0;

int i = 0;

/* cr'eation de la socket */
if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) 
	{
  	perror("socket"); 
  	exit(1);
  	}


/* recherche de l'@ IP de la machine distante */
for(int i; i<nbClients; i++){
if ((hote[i] = gethostbyname(adresseClients[i])) == NULL)
		{
  		perror("gethostbyname"); 
  		close(sock); 
  		exit(2);
  		}
}


while (choix=="o" && compteur<= nbClients){
	if ((recu = recvfrom(sock, &choix1,sizeof(char),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale)) != sizeof(char)) 
		{
		perror("recvfrom repVendeur"); 
		close(sock); 
		exit(1);
		}
if(choix1=="o"){printf("une personne est rentrée dans la vente, voulez vous continuer (il y a actuellement %d acheteurs potentiels)?",compteur);
scanf("y/n pour yes no",&choix);
if ((envoye = sendto(sock,&choix,sizeof(char),0,(struct sockaddr*)&adresseVente,lgadressevente)) != sizeof(char)) 
	{
  	perror("sendto choix"); 
  	close(sock); 
  	exit(1);
  	}
}
compteur=compteur+1;

}
/* on passe à la suite uniquement quand le vendeur refuse une première personne*/


/* pr'eparation de l'adresse locale : port + toutes les @ IP */
adresseLocale.sin_family = AF_INET;
adresseLocale.sin_port = htons(atoi(argv[1]));
adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);


/* attachement de la socket a` l'adresse locale */
lgadresseLocale = sizeof(adresseLocale);
if ((bind(sock,(struct sockaddr*)&adresseLocale,lgadresseLocale)) == -1) 
	{
  	perror("bind"); 
  	exit(1);
  	}


/* pr'eparation de l'adresse distante : port + la premier @ IP */

for(int j=0; j <= i; j++) {
adresseClients[j].sin_family = AF_INET;
adresseClients[j].sin_port = htons(atoi(argv[1]));
bcopy(hote->h_addr, &adresseClients[j].sin_addr, hote->h_length);
printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseClients[j].sin_addr));
}

for(int j=0; j<nbclients;j++){
lgadresseClients[j] = sizeof(adresseClients[j]);
}

/* saisie de la description et de la mise à prix initiale */
printf("Saisir la description de la vente : ");
scanf("%s", desc);

printf("Saisir la mise à prix initiale : ");
scanf("%d", offre);

//envoi de la description et du prix initial
for(int j=0; j < i-1; j++) {
        envoyeDesc = sendto(sock,&desc,sizeof(char *),0,(struct sockaddr*)&adresseClients[j],lgadresseClients[j]);
        envoyePrixInit = sendto(sock,&prixinit,sizeof(long),0,(struct sockaddr*)&adresseClients[j],lgadresseClients[j]);
        printf("\n | %s - %d | \n",desc, prixinit);
}

offreLaPlusElevee = offre;

/* reception d'une offre */
char ventecontinue = 'o';
while(ventecontinue == 'o'){
    //envoi du prix actuel
    for(int j=0; j < i-1; j++) {
        envoyeNouvelleOffre = sendto(sock,&offreLaPlusElevee,sizeof(long),0,(struct sockaddr*)&adresseClients[j],lgadresseClients[j]);
        printf("\n | %d | \n", offreLaPlusElevee);
    }
    //attente d'une nouvelle offre
    while(offre <= offreLaPlusElevee && cpt <= nbacheteurs){
        recu[j] = recvfrom(sock, &offre,sizeof(long),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale);
        cpt++;
    }
    //vendeur veut-il continuer ou l'offre lui convient ?
    offreLaPlusElevee = offre;
    printf("Mise actuelle : %d",offreLaPlusElevee);
    printf("La vente aux enchères continue ? o/n ");
    scanf("%c",ventecontinue);
}

char vide = '';

for(int j=0; j < i-1; j++) {//envoi du prix actuel
    envoyeVoid = sendto(sock,&vide,0,0,(struct sockaddr*)&adresseClients[j],lgadresseClients[j]);
    envoyeDerniereOffre = sendto(sock,&offreLaPlusElevee,sizeof(long),0,(struct sockaddr*)&adresseClients[j],lgadresseClients[j]);
    printf("\n | %d | \n", offreLaPlusElevee);
}

printf("\n OK\n");


//fermeture
close(sock);


}
