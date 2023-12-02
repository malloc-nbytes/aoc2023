let words = Hashtbl.create 9
let _ = Hashtbl.add words "one" '1'
let _ = Hashtbl.add words "two" '2'
let _ = Hashtbl.add words "three" '3'
let _ = Hashtbl.add words "four" '4'
let _ = Hashtbl.add words "five" '5'
let _ = Hashtbl.add words "six" '6'
let _ = Hashtbl.add words "seven" '7'
let _ = Hashtbl.add words "eight" '8'
let _ = Hashtbl.add words "nine" '9'

let file_to_str filename =
  let ch = open_in_bin filename in
  let s = really_input_string ch (in_channel_length ch) in
  close_in ch;
  s
;;

let rec search_word lst right =
  let is_word str =
    try Some (Hashtbl.find words str)
    with Not_found -> None in
  if lst = [] then None
  else
    match is_word (if right then lst |> List.to_seq |> String.of_seq
                   else lst |> List.rev |> List.to_seq |> String.of_seq)
    with
    | Some k -> Some k
    | None -> search_word (List.tl lst) right
;;

let rec find_digit (lst : char list) (acc : char list) (right : bool) : char =
  let isnum c =
    let c = int_of_char c in
    let c = c - int_of_char '0' in
    (c >= 0) && (c <= 9) in
  match search_word acc right with
  | Some num -> num
  | None ->
     (match lst with
      | [] -> failwith "no numbers found"
      | hd :: tl when isnum hd -> hd
      | hd :: tl -> find_digit tl (acc @ [hd]) right)
;;

let filepath = "./input.txt"
let () =
  let lst = file_to_str filepath
            |> String.split_on_char '\n'
            |> List.rev |> List.tl |> List.rev in

  let lst = List.map (fun (line : string) ->
                let line = line |> String.to_seq |> List.of_seq in
                let x = find_digit line [] true in
                let y = find_digit (List.rev line) [] false in
                int_of_string (String.make 1 x ^ String.make 1 y)) lst in

  print_int (List.fold_left (fun acc x -> acc + x) 0 lst)
;;
