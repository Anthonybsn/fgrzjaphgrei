# ESP32 Web Control

Projet permettant de contrôler une LED et un buzzer avec un ESP32 depuis une interface web.

---

# Fonctionnement du projet

Le projet utilise :

- un ESP32 connecté au Wi-Fi
- un serveur web intégré
- une interface web en PHP
- des requêtes HTTP

L’utilisateur clique sur un bouton du site web et l’ESP32 reçoit la commande.

---

# Structure du projet

```txt
ESP32-Web-Control/
│
├── esp32.ino
├── data.php
├── index.php
├── valeur.txt
├── valeur.csv
└── README.md
```

---

# 1. Code ESP32

Fichier : `esp32.ino`

```cpp
#include <WiFi.h>
#include <WebServer.h>

// Informations WiFi
const char* ssid = "TPSN035";
const char* password = "BTSSN2022";

// Création du serveur web
WebServer server(80);

// Déclaration des broches
int sortieled = 23;
int buzzer = 26;

// Fréquence du buzzer
int frequence = 2000;


// ======================================================
// Fonction : Allumer la LED
// ======================================================
void gestionLED() {

  digitalWrite(sortieled, HIGH);

  server.send(200, "text/plain", "LED allumee");
}


// ======================================================
// Fonction : Éteindre la LED
// ======================================================
void gestionLEDoff() {

  digitalWrite(sortieled, LOW);

  server.send(200, "text/plain", "LED eteinte");
}


// ======================================================
// Fonction : Activer le buzzer
// ======================================================
void gestionSON() {

  ledcWriteTone(buzzer, 1000);
  delay(500);

  ledcWriteTone(buzzer, 500);
  delay(500);

  ledcWriteTone(buzzer, 1000);
  delay(500);

  ledcWriteTone(buzzer, 500);
  delay(500);

  server.send(200, "text/plain", "Son active");
}


// ======================================================
// Fonction : Arrêter le buzzer
// ======================================================
void gestionSONoff() {

  ledcWriteTone(buzzer, 0);

  server.send(200, "text/plain", "Son arrete");
}


// ======================================================
// SETUP
// ======================================================
void setup() {

  // Démarrage du moniteur série
  Serial.begin(115200);

  // LED configurée comme sortie
  pinMode(sortieled, OUTPUT);

  // Configuration du buzzer
  ledcAttach(buzzer, frequence, 8);

  // Connexion WiFi
  WiFi.begin(ssid, password);

  // Attente de la connexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Affichage adresse IP
  Serial.println(WiFi.localIP());

  // Routes du serveur
  server.on("/led", gestionLED);

  server.on("/ledoff", gestionLEDoff);

  server.on("/son", gestionSON);

  server.on("/sonoff", gestionSONoff);

  // Démarrage serveur
  server.begin();
}


// ======================================================
// LOOP
// ======================================================
void loop() {

  // Gestion des clients connectés
  server.handleClient();
}
```

---

# Explication du code ESP32

## Bibliothèques

```cpp
#include <WiFi.h>
#include <WebServer.h>
```

### Explication

- `WiFi.h` permet de connecter l’ESP32 au réseau Wi-Fi
- `WebServer.h` permet de créer un serveur web sur l’ESP32

---

## Configuration Wi-Fi

```cpp
const char* ssid = "TPSN035";
const char* password = "BTSSN2022";
```

### Explication

Ces variables contiennent :

- le nom du réseau Wi-Fi
- le mot de passe

L’ESP32 utilise ces informations pour se connecter au réseau.

---

## Création du serveur

```cpp
WebServer server(80);
```

### Explication

Création d’un serveur web sur le port `80`.

Le port 80 est le port utilisé par les sites web HTTP.

---

## Déclaration des broches

```cpp
int sortieled = 23;
int buzzer = 26;
```

### Explication

- GPIO 23 → LED
- GPIO 26 → Buzzer

---

## Fonction `gestionLED()`

```cpp
digitalWrite(sortieled, HIGH);
```

### Explication

Cette ligne met la broche GPIO 23 à HIGH.

La LED s’allume.

---

```cpp
server.send(200, "text/plain", "LED allumee");
```

### Explication

Le serveur envoie une réponse HTTP :

- `200` = succès
- `"text/plain"` = texte simple
- `"LED allumee"` = message envoyé

---

## Fonction `gestionLEDoff()`

```cpp
digitalWrite(sortieled, LOW);
```

### Explication

Met la broche GPIO 23 à LOW.

La LED s’éteint.

---

## Fonction `gestionSON()`

```cpp
ledcWriteTone(buzzer, 1000);
```

### Explication

Le buzzer joue un son à 1000 Hz.

---

```cpp
delay(500);
```

### Explication

Le programme attend 500 millisecondes.

---

## Fonction `gestionSONoff()`

```cpp
ledcWriteTone(buzzer, 0);
```

### Explication

Arrête complètement le buzzer.

---

## Fonction `setup()`

```cpp
void setup()
```

### Explication

La fonction `setup()` s’exécute une seule fois au démarrage de l’ESP32.

---

## Moniteur série

```cpp
Serial.begin(115200);
```

### Explication

Active la communication série.

Permet d’afficher des informations dans Arduino IDE.

---

## Configuration LED

```cpp
pinMode(sortieled, OUTPUT);
```

### Explication

Configure la broche GPIO 23 comme sortie.

---

## Connexion Wi-Fi

```cpp
WiFi.begin(ssid, password);
```

### Explication

L’ESP32 tente de se connecter au Wi-Fi.

---

## Vérification connexion

```cpp
while (WiFi.status() != WL_CONNECTED)
```

### Explication

Le programme attend jusqu’à la connexion au réseau.

---

## Adresse IP

```cpp
Serial.println(WiFi.localIP());
```

### Explication

Affiche l’adresse IP de l’ESP32.

Exemple :

```txt
192.168.100.123
```

---

## Routes serveur

```cpp
server.on("/led", gestionLED);
```

### Explication

Quand l’utilisateur visite :

```txt
http://IP_ESP32/led
```

la fonction `gestionLED()` est exécutée.

---

## Fonction loop()

```cpp
server.handleClient();
```

### Explication

Le serveur vérifie constamment si un client envoie une requête.

---

# 2. Code PHP

Fichier : `data.php`

```php
<?php

error_reporting(E_ALL);
ini_set('display_errors', 1);

date_default_timezone_set("Pacific/Noumea");

$txtFile = __DIR__ . "/valeur.txt";
$csvFile = __DIR__ . "/valeur.csv";

if ($_SERVER['REQUEST_METHOD'] === 'POST') {

    if (isset($_POST['valeur'])) {

        $valeur = $_POST['valeur'];

        file_put_contents($txtFile, $valeur);

        $date = date("Y-m-d");
        $heure = date("H:i:s");

        $fichier = fopen($csvFile, "a");

        fputcsv($fichier, [$date, $heure, $valeur]);

        fclose($fichier);

        echo "OK";

    } else {

        echo "POST reçu mais pas de valeur";
    }

    exit;
}
?>
```

---

# Explication du code PHP

## Affichage des erreurs

```php
error_reporting(E_ALL);
```

### Explication

Affiche toutes les erreurs PHP.

Très utile pour le débogage.

---

## Fuseau horaire

```php
date_default_timezone_set("Pacific/Noumea");
```

### Explication

Configure l’heure de Nouvelle-Calédonie.

---

## Création des fichiers

```php
$txtFile = __DIR__ . "/valeur.txt";
```

### Explication

Chemin du fichier texte.

---

## Vérification POST

```php
if ($_SERVER['REQUEST_METHOD'] === 'POST')
```

### Explication

Le script vérifie si une requête POST est reçue.

---

## Lecture valeur

```php
$valeur = $_POST['valeur'];
```

### Explication

Récupère la valeur envoyée.

---

## Sauvegarde TXT

```php
file_put_contents($txtFile, $valeur);
```

### Explication

Sauvegarde la valeur dans `valeur.txt`.

---

## Sauvegarde CSV

```php
fputcsv($fichier, [$date, $heure, $valeur]);
```

### Explication

Ajoute une ligne dans le fichier CSV.

---

# 3. Interface Web

Fichier : `index.php`

```php
<?php

$valeur = "Aucune donnée";

if (file_exists("valeur.txt")) {
    $valeur = file_get_contents("valeur.txt");
}

?>

<!DOCTYPE html>
<html lang="fr">

<head>

<meta charset="UTF-8">

<meta http-equiv="refresh" content="2">

<title>ESP32 Web Control</title>

</head>

<body>

<h1>Valeur reçue depuis l'ESP32</h1>

<div>
<?php echo htmlspecialchars($valeur); ?>
</div>

<h1>Commande ESP32 Son</h1>

<button onclick="fetch('http://192.168.100.123/son')">
Allumer
</button>

<button onclick="fetch('http://192.168.100.123/sonoff')">
Éteindre
</button>

<h1>Commande ESP32 LED</h1>

<button onclick="fetch('http://192.168.100.123/led')">
Allumer
</button>

<button onclick="fetch('http://192.168.100.123/ledoff')">
Éteindre
</button>

</body>
</html>
```

---

# Explication du site web

## Lecture du fichier

```php
file_get_contents("valeur.txt");
```

### Explication

Lit la dernière valeur enregistrée.

---

## Rafraîchissement automatique

```html
<meta http-equiv="refresh" content="2">
```

### Explication

Recharge la page toutes les 2 secondes.

---

## Bouton LED

```html
fetch('http://192.168.100.123/led')
```

### Explication

Envoie une requête HTTP à l’ESP32.

La LED s’allume.

---

## Bouton buzzer

```html
fetch('http://192.168.100.123/son')
```

### Explication

Active le buzzer à distance.

---

# Matériel utilisé

- ESP32
- LED
- Résistance 220Ω
- Buzzer
- Breadboard
- Câbles Dupont

---

# Connexions

| Composant | GPIO |
|---|---|
| LED | GPIO 23 |
| Buzzer | GPIO 26 |

---

# Fonctionnalités

✅ Contrôle LED  
✅ Contrôle buzzer  
✅ Serveur web ESP32  
✅ Interface PHP  
✅ Communication Wi-Fi  
✅ Sauvegarde CSV  
