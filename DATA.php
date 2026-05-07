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
