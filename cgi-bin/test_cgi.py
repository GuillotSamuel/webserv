import re

# Chemin vers le fichier contenant la requête HTTP
input_file_path = '/home/user/ecole_42/webserv/tmp.txt'

# Lire le contenu du fichier
with open(input_file_path, 'r') as file:
    content = file.read()

# Définir un dictionnaire pour stocker les données du formulaire
form_data = {}

# Utiliser des expressions régulières pour extraire les données du formulaire
pattern = r'Content-Disposition: form-data; name="(.*?)"\r?\n\r?\n(.*?)\r?\n'
matches = re.findall(pattern, content, re.DOTALL)

# Remplir le dictionnaire avec les données extraites
for match in matches:
    field_name = match[0]
    field_value = match[1]
    form_data[field_name] = field_value.strip()

# Extraire les valeurs des champs
first_name = form_data.get('first-name', 'Inconnu')
last_name = form_data.get('last-name', 'Inconnu')
favorite_color = form_data.get('favorite-color', 'Inconnu')

# Générer le contenu HTML
print (f"""
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Données du formulaire</title>
</head>
<body>
    <h1>Données du formulaire</h1>
    <p><strong>Prénom :</strong> {first_name}</p>
    <p><strong>Nom :</strong> {last_name}</p>
    <p><strong>Couleur préférée :</strong> {favorite_color}</p>
</body>
</html>
""")