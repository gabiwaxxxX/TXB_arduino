Pour tester la validitee de ce code il faut :

Recuperer la derniere version de l' api sur le github https://github.com/gabrielfournier1/Bennes_Decheterie 

Ensuite dans le dossier aller dans -> 2/api/ executer la commande ./mvnw clean package qui va regnerer un fichier jar dans le dossier api/target

Vous pourez ensuite demarer l'api simplement grace a la commande java -jar api-jacla-api-0.0.1-SNAPSHOT.jar dans le dossier api/target

Il faut aussi ouvirir un serveur Redis. Celui-ci se trouve dans le dossier 2/redis/src du github precedent et executer la commande redis-server



Cote ESP il faut charger le code. 

Dans un premier temps apres qu'il soit allumer il va creer un hotspot qui sera detecable par vos machines -> Reseau : ESP32AP code : esp32test.

Ensuite il faut indiquer le reseau sur lequel votre machine avec l'API et la BD est connecte. L'ESP sera connecter a votre reseau. 

Une fois cela etablie. Sur l'API il faut appyuer sur le bouton Apparaillage qui va lancer une analyse des adresses IP du reseau et envoyer un message a chaqu' un d'elles.
L' ESP est en attente de ce message (Message Erorr -1) . Ensuite si c' est la premiere fois qu'il se connecte a la BD il va enyoyer un fichier JSON pour montrer son identiter. 



