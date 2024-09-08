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