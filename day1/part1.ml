let file_to_str filename =
  let ch = open_in_bin filename in
  let s = really_input_string ch (in_channel_length ch) in
  close_in ch;
  s

let rec find_digit (lst : char list) : char =
  let isnum c =
    let c = int_of_char c in
    let c = c - int_of_char '0' in
    (c >= 0) && (c <= 9) in
  match lst with
  | [] -> failwith "invalid input"
  | hd :: tl when isnum hd -> hd
  | hd :: tl -> find_digit tl

let filepath = "./input.txt"
let () =
  let lst = file_to_str filepath
            |> String.split_on_char '\n'
            |> List.rev |> List.tl |> List.rev in

  let lst = List.map (fun (line : string) ->
                let line = line |> String.to_seq |> List.of_seq in
                let x = find_digit line in
                let y = find_digit @@ List.rev line in
                int_of_string (String.make 1 x ^ String.make 1 y)) lst in

  print_int (List.fold_left (fun acc x -> acc + x) 0 lst)
