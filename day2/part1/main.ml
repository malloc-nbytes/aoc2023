let file_to_str filename =
  let ch = open_in_bin filename in
  let s = really_input_string ch (in_channel_length ch) in
  close_in ch;
  s

let rcap = 12
let gcap = 13
let bcap = 14

let rec possible lst =
  let lst = lst
            |> String.to_seq |> List.of_seq
            |> List.filter (fun c -> c <> ';' && c <> ',' && c <> ':')
            |> List.to_seq |> String.of_seq
            |> String.split_on_char ' ' in
  let id, lst = int_of_string (List.hd (List.tl lst)), List.tl (List.tl lst) in
  let rec aux lst =
    match lst with
    | [] -> true
    | x :: xs :: tl ->
       (match xs with
        | "red" -> if int_of_string x > rcap then false else aux tl
        | "green" -> if int_of_string x > gcap then false else aux tl
        | "blue" -> if int_of_string x > bcap then false else aux tl
        | _ -> failwith "invalid word")
    | hd :: [] -> true in
  if aux lst then id else 0

let filepath = "./input.txt"
let () =
  let lst = file_to_str filepath
             |> String.split_on_char '\n'
             |> List.filter (fun k -> k <> "") in
  let rec loop lst =
    match lst with
    | [] -> 0
    | hd :: tl -> loop tl + possible hd in
  print_int @@ loop lst
