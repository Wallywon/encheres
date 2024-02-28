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
/* Actions a faire pour la partie 1.3*/
}

int main (int argc, char **argv)
{
int sock,recu,envoye;
struct sockaddr_in adresseLocale;
int lgadresseLocale;
struct sigaction action;
char choix; //o ou n pour repondre aux questions 
char repVendeur; //o ou n pour me repondre 
char description[500];
long prix;
/* handler de signal SIGINT */
action.sa_handler = arretParControlC;
sigaction(SIGINT, &action, NULL);

struct sockaddr_in adresseVente;
adresseVente="14DE016vm";
/* cr'eation de la socket */
if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) 
	{
  	perror("socket"); 
  	exit(1);
  	}

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

/*preparation de l'adresse de la vente*/
if ((hote = gethostbyname(adresseVente)) == NULL)
	{
  	perror("gethostbyname"); 
  	close(sock); 
  	exit(2);
  	}
/* pr'eparation de l'adresse distante : port + la premier @ IP */
adresseVente.sin_family = AF_INET;
adresseVente.sin_port = htons(atoi(argv[1]));
bcopy(hote->h_addr, &adresseVente.sin_addr, hote->h_length);
printf("L'adresse de la vente est : %s\n", inet_ntoa(adresseVente.sin_addr));


/*envoie de la demande de participation*/

printf("voulez vous participer à la vente? (oui/non)");
scanf("%c",&choix);
if(choix=="n"){close(sock); exit(-1);}
/*réponse du vendeur (o=oui/ n=non) pour participer*/
lgadressevente = sizeof(adresseVente);
if ((envoye = sendto(sock,&choix,sizeof(char),0,(struct sockaddr*)&adresseVente,lgadressevente)) != sizeof(char)) 
	{
  	perror("sendto choix"); 
  	close(sock); 
  	exit(1);
  	}
/*reception de l'acceptation ou du refus de participation par l'acheteur*/

if ((recu = recvfrom(sock, &repVendeur,sizeof(char),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale)) != sizeof(char)) 
	{
	perror("recvfrom repVendeur"); 
	close(sock); 
	exit(1);
	}
/*si le vendeur ne veut pas que je participe il repond n*/
if(repVendeur=="n"){close(sock);exit(-1);}
/*sinon je suis mis en attente du début de la vente*/
/*je recois la descrition*/





if(repVendeur=="y"){

if ((recu = recvfrom(sock, &description,sizeof(char*),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale)) != sizeof(char*)) 
	{
	perror("recvfrom description"); 
	close(sock); 
	exit(1);
	}

printf("le produit mis en vente aujourd'hui est : \n %s \n",description);
/*je recois le prix initial*/
if ((recu = recvfrom(sock, &prix,sizeof(long),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale)) != sizeof(long)) 
	{
	perror("recvfrom prix"); 
	close(sock); 
	exit(1);
	}
printf("le produit mis en vente aujourd'hui à pour prix initial: \n %lg \n",prix);
while(recu>0){
printf("si vous voulez faire une offre notez le prix que vous proposez. Sinon tapez 0");
scanf("%lg",&proposition);
if ((envoye = sendto(sock,&proposition,sizeof(long),0,(struct sockaddr*)&adresseVente,lgadressevente)) != sizeof(long)) 
	{
  	perror("sendto choix"); 
  	close(sock); 
  	exit(1);
  	}
if ((recu = recvfrom(sock, &prix,sizeof(long),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale)) != sizeof(long)) 
	{
	perror("recvfrom prix"); 
	close(sock); 
	exit(1);
	}
if(recu!=0){printf("\n offre actuelle: %lg \n",prix);}
else{printf("la vente est terminée, en attente des resultats...");}
}

if ((recu = recvfrom(sock, &prix,sizeof(long),0,(struct sockaddr*)&adresseLocale,&lgadresseLocale)) != sizeof(long)) 
	{
	perror("recvfrom prix"); 
	close(sock); 
	exit(1);
	}
printf("\n offre finale: %lg \n",prix);

if(prix==proposition){printf("Feliciation vous avez gagné ");}
else{printf("Vous n'avez pas gagné cette fois ci");}
}

close(sock);
}
