#!/usr/bin/env python3

import cgi
import os

# Définir le répertoire où les fichiers seront téléchargés (racine du serveur web)
UPLOAD_DIR = "/home/user/ecole_42/webserv"  # Remplace par le chemin réel de la racine du serveur

# Lire la requête à partir du fichier request.txt (facultatif si utilisé différemment)
with open('request.txt', 'r') as request_file:
    request_data = request_file.read()
    print("Requête lue depuis request.txt :")
    print(request_data)

# Créer un objet de formulaire pour traiter les données envoyées
form = cgi.FieldStorage()

# Extraire les champs de texte
first_name = form.getvalue('first-name')
last_name = form.getvalue('last-name')
favorite_color = form.getvalue('favorite-color')

# Extraire et traiter l'image
if 'image' in form:
    fileitem = form['image']
    
    # Vérifier si un fichier a bien été téléchargé
    if fileitem.filename:
        # Extraire le nom du fichier
        filename = os.path.basename(fileitem.filename)
        
        # Définir le chemin complet où l'image sera enregistrée
        filepath = os.path.join(UPLOAD_DIR, filename)
        
        # Écrire le fichier image à l'emplacement défini
        with open(filepath, 'wb') as f:
            f.write(fileitem.file.read())
        


    import sys
import io
import cgi

# Simuler un fichier temporaire contenant la requête POST
with open("/path/to/your/temporary_file", "rb") as temp_file:
    # Rediriger le contenu du fichier temporaire vers stdin
    sys.stdin = io.StringIO(temp_file.read().decode('utf-8'))  # Lire en mode texte si c'est UTF-8
    
    # Utiliser FieldStorage pour parser la requête
    form = cgi.FieldStorage()

    # Récupérer les données du formulaire comme d'habitude
    if "first-name" in form and "last-name" in form:
        first_name = form.getvalue("first-name")
        last_name = form.getvalue("last-name")
        print(f"First Name: {first_name}")
        print(f"Last Name: {last_name}")
    
    # Gérer les fichiers uploadés
    if "image" in form:
        fileitem = form["image"]
        if fileitem.filename:
            filename = os.path.basename(fileitem.filename)
            with open(f"/tmp/{filename}", 'wb') as f:
                f.write(fileitem.file.read())
            print(f"File {filename} has been uploaded.")
        else:
            print("No file uploaded.")
        # Réponse après soumission réussie
        print(f"Content-type: text/html\n")
        print("<html><body>")
        print(f"<h1>Merci {first_name} {last_name}!</h1>")
        print(f"<p>Votre couleur préférée est {favorite_color}.</p>")
        print(f"<p>Image téléchargée avec succès sous le nom : {filename}</p>")
        print("</body></html>")
    else:
        # Si aucun fichier n'est téléchargé
        print(f"Content-type: text/html\n")
        print("<html><body>")
        print("<h1>Aucune image téléchargée !</h1>")
        print("</body></html>")
else:
    # Si le champ image n'existe pas
    print(f"Content-type: text/html\n")
    print("<html><body>")
    print("<h1>Le champ image est manquant dans le formulaire !</h1>")
    print("</body></html>")