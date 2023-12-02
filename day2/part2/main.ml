let file_to_str filename =
  let ch = open_in_bin filename in
  let s = really_input_string ch (in_channel_length ch) in
  close_in ch;
  s

let rec possible lst =
  let lst = lst
            |> String.to_seq |> List.of_seq
            |> List.filter (fun c -> c <> ';' && c <> ',' && c <> ':')
            |> List.to_seq |> String.of_seq
            |> String.split_on_char ' ' in
  let id, lst = int_of_string (List.hd (List.tl lst)), List.tl (List.tl lst) in
  let rec aux lst rm gm bm =
    match lst with
    | [] -> rm, gm, bm
    | x :: xs :: tl ->
       let x = int_of_string x in
       (match xs with
        | "red" -> aux tl (if x > rm then x else rm) gm bm
        | "green" -> aux tl rm (if x > gm then x else gm) bm
        | "blue" -> aux tl rm gm (if x > bm then x else bm)
        | _ -> failwith "invalid word")
    | hd :: [] -> rm, gm, bm in
  let rm, gm, bm = aux lst 0 0 0 in
  rm * gm * bm

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
