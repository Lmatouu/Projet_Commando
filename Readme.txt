Robot V2    Mathis VEBER

##Description

Ce projet vise à envoyer des instructions depuis une télécommande pour contrôler le mouvement du robot dans un simulateur.

##Compilation

Placez la bibliothèque "mrpiz" dans le dossier "VEBER_ROBOT_V2", au même niveau que "telco" et "commando". Ensuite, à partir de ce dossier, ouvrez un terminal et exécutez la commande suivante :

make

##Exécutable
Lancer d'abord le logiciel robot avec cette commande dans le terminal à l'emplacement du fichier .jar
java -jar intox_v0.5-mrpiz_v0.2-java_1.9.jar

Puis pour lancer les programmes, exécutez les commandes suivantes dans deux terminaux situés dans le même dossier :

1er terminal :

sudo ./commando/bin/go

2ème terminal :

sudo ./telco/bin/go

##Utilisation

Dans le 2ème terminal qui gère "telco", voici les commandes du clavier disponibles et leurs actions :

'Z' permet de faire avancer le Robot
'Q' permet de faire tourner le Robot vers la gauche
'D' permet de faire un demi-tour équivalent à tourner à gauche
'S' permet de faire tourner le Robot vers la droite
'Espace' permet de détecter la présence d'obstacle(s)
'M' permet d'afficher les différentes interactions possibles avec la console
'O' permet de stopper le Robot et stopper l'éxécutable



