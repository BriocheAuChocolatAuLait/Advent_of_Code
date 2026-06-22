let file = open_in "input.exe"

(*Appliqué à une colonne elle permet d'avoir le compte d'étoile dedans*)
let count_column column = 
  let rec aux l acc = 
    match l with
    |'#'::t -> aux t (acc + 1)
    |h::t-> aux t acc
    |[] -> acc
  in aux column (-1)

(*Fonction pour récuperer un bloc et le diviser en une liste de liste de colonne*)
let divide_in_column block = 
  let length = String.length (List.hd block) in 
  let convert i = 
    List.map (fun line -> line.[i]) block 
  in 
  List.init length convert

(*Appel aux fonctions sur un block et renvoie une liste avec le compte de chaque #*)
let block_to_list block = 
  divide_in_column block
  |> List.map count_column

let parse file = 
  let rec loop keys locks current_pattern is_lock = 
  try 
    let line = input_line file in 
    if line = "" 
      then if is_lock 
        then loop keys (block_to_list (List.rev current_pattern)::locks) [] false
    else loop (block_to_list (List.rev current_pattern)::keys) locks [] false
  else if current_pattern = [] && line = "#####" 
    then loop keys locks ["#####"] true 
 else loop keys locks (line::current_pattern) is_lock
with End_of_file ->
  close_in file;
  if is_lock then keys,block_to_list (List.rev current_pattern)::locks
  else block_to_list (List.rev current_pattern)::keys, locks
in loop [] [] [] false

let keys, lock = parse file

let rec check_list key lock =
  match key,lock with 
  |[],[] -> 1
  |h::t,h2::t2 when h + h2 < 6 -> check_list t t2 
  |_ -> 0

let rec check key locks =
  match locks with 
  |[] -> 0
  |h::t -> check_list key h + check key t

let rec main couple = 
  match couple with 
  |[],_ -> 0
  |h::t, locks -> check h locks + main (t,locks)