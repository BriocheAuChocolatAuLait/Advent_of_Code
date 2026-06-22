let lire_fichier nom_fichier =
  let ic = open_in nom_fichier in
  let towels_disp = 
    input_line ic 
    |> String.split_on_char ',' 
    |> List.map String.trim in
  let rec lire_lignes acc =
    try
      let ligne = input_line ic in
      lire_lignes (ligne::acc) 
    with End_of_file ->
      close_in ic;
      towels_disp,acc
  in
  lire_lignes []


let towels_dispo, towels_to_make = lire_fichier "/home/briac/Documents/OCaml/input.txt"

let can_construct design towels =
let len = String.length design in
let cache = Hashtbl.create len in

let rec aux i =
  if i = len then true
  else if Hashtbl.mem cache i then Hashtbl.find cache i
  else
    let possible = List.exists (fun towel ->
      let tl = String.length towel in
      i + tl <= len && String.sub design i tl = towel && aux (i + tl)
    ) towels in
    Hashtbl.add cache i possible;
    possible
in
aux 0

let count_possible_designs towels designs =
  List.fold_left (fun acc design -> if can_construct design towels then acc + 1 else acc) 0 designs;;

let ()=
let result = count_possible_designs towels_dispo towels_to_make in
Printf.printf "%d\n" result;;