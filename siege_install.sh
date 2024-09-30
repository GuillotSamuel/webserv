# Se mettre dans le dossier Desktop (A modifier si necessaire)
cd ~/Desktop

# Télécharger Siege sans vérifier le certificat
wget --no-check-certificate https://download.joedog.org/siege/siege-latest.tar.gz

# Extraire l'archive
tar -xvzf siege-latest.tar.gz

# Se déplacer dans le dossier extrait
cd siege-*/

# Configurer l'installation avec un chemin local
./configure --prefix=$HOME/.local

# Compiler et installer Siege
make
make install

# Ajouter le chemin d'installation au PATH
echo 'export PATH=$HOME/.local/bin:$PATH' >> ~/.bashrc
source ~/.bashrc

# Vérifier l'installation
siege --version
