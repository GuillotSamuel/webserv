import re

# Chemin vers le fichier contenant la requête HTTP
input_file_path = '/home/user/ecole_42/webserv/request.txt'

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

if image_filename:
    print(f"""
    <h2>Image téléchargée :</h2>
    <img src="{image_filename}" alt="Image téléchargée" />
    """)

print("""
</body>
</html>
""")

# import re

# # Chemin vers le fichier contenant la requête HTTP
# input_file_path = '/home/user/ecole_42/webserv/request.txt'

# # Lire le contenu du fichier
# with open(input_file_path, 'r') as file:
#     content = file.read()

# # Définir un dictionnaire pour stocker les données du formulaire
# form_data = {}

# # Utiliser des expressions régulières pour extraire les données du formulaire
# pattern = r'Content-Disposition: form-data; name="(.*?)"(?:; filename="(.*?)")?\r?\nContent-Type: (.*?)\r?\n\r?\n(.*?)\r?\n'
# matches = re.findall(pattern, content, re.DOTALL)

# # Variable pour stocker le nom de fichier de l'image et son contenu
# image_filename = None
# image_content = None

# # Remplir le dictionnaire avec les données extraites
# for match in matches:
#     field_name = match[0]
#     filename = match[1]
#     content_type = match[2]
#     field_value = match[3]

#     if filename:
#         # Si un nom de fichier est présent, c'est une image ou un fichier binaire
#         image_filename = filename
#         image_content = field_value
#     else:
#         # Sinon, c'est un champ de formulaire normal
#         form_data[field_name] = field_value.strip()

# # Sauvegarder l'image si elle existe
# if image_filename and image_content:
#     with open(image_filename, 'wb') as image_file:
#         image_file.write(image_content.encode('latin1'))  # Convertir en bytes et écrire

# # Extraire les valeurs des champs
# first_name = form_data.get('first-name', 'Inconnu')
# last_name = form_data.get('last-name', 'Inconnu')
# favorite_color = form_data.get('favorite-color', 'Inconnu')

# # Générer le contenu HTML
# print(f"""
# <!DOCTYPE html>
# <html lang="fr">
# <head>
#     <meta charset="UTF-8">
#     <title>Données du formulaire</title>
# </head>
# <body>
#     <h1>Données du formulaire</h1>
#     <p><strong>Prénom :</strong> {first_name}</p>
#     <p><strong>Nom :</strong> {last_name}</p>
#     <p><strong>Couleur préférée :</strong> {favorite_color}</p>
# """)

# # Afficher l'image si elle a été téléchargée
# if image_filename:
#     print(f"""
#     <h2>Image téléchargée :</h2>
#     <img src="{image_filename}" alt="Image téléchargée" />
#     """)

# print("""
# </body>
# </html>
# """)

# import re

# # Chemin vers le fichier contenant la requête HTTP
# input_file_path = '/home/user/ecole_42/webserv/request.txt'

# # Lire le contenu du fichier en mode binaire
# with open(input_file_path, 'rb') as file:
#     content = file.read()

# # Définir un dictionnaire pour stocker les données du formulaire
# form_data = {}

# # Utiliser des expressions régulières pour extraire les données du formulaire
# # On doit décoder les bytes en utilisant la gestion correcte des retours à la ligne et des délimiteurs de contenu
# pattern = b'Content-Disposition: form-data; name="(.*?)"(?:; filename="(.*?)")?\r?\n(?:Content-Type: (.*?))?\r?\n\r?\n(.*?)(?=\r?\n--(?:\r?\n|$))'
# matches = re.findall(pattern, content, re.DOTALL)

# # Variable pour stocker le nom de fichier de l'image et son contenu
# image_filename = None
# image_content = None

# # Remplir le dictionnaire avec les données extraites
# for match in matches:
#     field_name = match[0].decode('utf-8')
#     filename = match[1].decode('utf-8') if match[1] else None
#     content_type = match[2].decode('utf-8') if match[2] else None
#     field_value = match[3]

#     if filename:
#         # Si un nom de fichier est présent, c'est une image ou un fichier binaire
#         image_filename = filename
#         image_content = field_value
#     else:
#         # Sinon, c'est un champ de formulaire normal
#         form_data[field_name] = field_value.decode('utf-8').strip()

# # Sauvegarder l'image si elle existe
# if image_filename and image_content:
#     with open(image_filename, 'wb') as image_file:
#         image_file.write(image_content)  # Écrire les bytes directement

# # Extraire les valeurs des champs
# first_name = form_data.get('first-name', 'Inconnu')
# last_name = form_data.get('last-name', 'Inconnu')
# favorite_color = form_data.get('favorite-color', 'Inconnu')

# # Générer le contenu HTML
# print(f"""
# <!DOCTYPE html>
# <html lang="fr">
# <head>
#     <meta charset="UTF-8">
#     <title>Données du formulaire</title>
# </head>
# <body>
#     <h1>Données du formulaire</h1>
#     <p><strong>Prénom :</strong> {first_name}</p>
#     <p><strong>Nom :</strong> {last_name}</p>
#     <p><strong>Couleur préférée :</strong> {favorite_color}</p>
# """)

# # Afficher l'image si elle a été téléchargée
# if image_filename:
#     print(f"""
#     <h2>Image téléchargée :</h2>
#     <img src="{image_filename}" alt="Image téléchargée" />
#     """)

# print("""
# </body>
# </html>
# """)