#!/usr/bin/env python3

import sys

def find_boundary_from_stdin():
    # Lire tout le contenu de stdin
    input_data = sys.stdin.read()
    
    # Chercher l'indice du début de "boundary="
    boundary_start = input_data.find("boundary=")
    
    if boundary_start == -1:
        # Si "boundary=" n'est pas trouvé, renvoyer une chaîne vide ou un message d'erreur
        return None
    
    # Début du boundary après "boundary="
    boundary_start += len("boundary=")
    
    # Trouver l'indice du prochain "\r\n" après "boundary="
    boundary_end = input_data.find("\r\n", boundary_start)
    
    if boundary_end == -1:
        # Si "\r\n" n'est pas trouvé, prendre la fin de la chaîne
        boundary_end = len(input_data)
    
    # Extraire et retourner la valeur du boundary
    boundary_value = input_data[boundary_start:boundary_end]
    return boundary_value.strip()

if __name__ == "__main__":
    boundary = find_boundary_from_stdin()
    if boundary:
        print(boundary)
    else:
        print("Boundary not found.")

