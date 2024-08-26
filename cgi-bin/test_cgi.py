#!/usr/bin/env python3

import cgi
import cgitb
import os

# Activer le mode de débogage CGI pour afficher les erreurs dans le navigateur
cgitb.enable()

# Définir le type de contenu renvoyé par le script
print("Content-Type: text/html\n")

# Récupérer les données du formulaire
form = cgi.FieldStorage()

# Extraire les valeurs des champs du formulaire
first_name = form.getvalue("first-name")
last_name = form.getvalue("last-name")
favorite_color = form.getvalue("favorite-color")
fileitem = form['image']

# Vérifier si un fichier a été téléchargé
if fileitem.filename:
    # Définir le chemin où l'image téléchargée sera stockée
    upload_dir = "/home/user/ecole_42/webserv3/uploads/"
    if not os.path.exists(upload_dir):
        os.makedirs(upload_dir)
    # Extraire le nom du fichier et le stocker
    filepath = os.path.join(upload_dir, os.path.basename(fileitem.filename))
    with open(filepath, 'wb') as f:
        f.write(fileitem.file.read())
    upload_message = f"File '{fileitem.filename}' uploaded successfully."
else:
    upload_message = "No file uploaded."

# Créer une réponse HTML dynamique
print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Form Submission Result</title>
    <link rel="stylesheet" href="../style.css">
</head>
<body>
    <header>
        <a class="header-title" href="index.html">Webserv</a>
    </header>
    <main>
        <h1>Form Submission Result</h1>
        <p>First Name: {first_name}</p>
        <p>Last Name: {last_name}</p>
        <p>Favorite Color: {favorite_color}</p>
        <p>{upload_message}</p>
        <a href="index.html">Go back to the form</a>
    </main>
    <footer>
        <div class="footer-credits">Project made by sguillot and mmahfoud</div>
    </footer>
</body>
</html>
""")