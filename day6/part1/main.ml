let file_to_str filename =
  let ch = open_in_bin filename in
  let s = really_input_string ch (in_channel_length ch) in
  close_in ch;
  s
;;

let parse_input input_str =
  let rec parse_numbers acc num_str = function
    | [] -> List.rev (int_of_string num_str :: acc)
    | hd :: tl ->
       let is_digit c = c >= '0' && c <= '9' in
       if is_digit hd then parse_numbers acc (num_str ^ String.make 1 hd) tl
       else if num_str = "" then parse_numbers acc "" tl
       else parse_numbers (int_of_string num_str :: acc) "" tl in
  parse_numbers [] "" @@ List.of_seq (String.to_seq input_str)
;;

let rec find_wins time dist i =
  match i > time with
  | true -> 0
  | _ ->
     let our_dist = i*(time-i) in
     if our_dist > dist then
       1 + find_wins (time) dist (i+1)
     else
       0 + find_wins (time) dist (i+1)

let filepath = "./input.txt"
let () =
  let data = String.split_on_char '\n' @@ file_to_str filepath in
  let times = parse_input @@ List.hd data
  and dists = parse_input @@ List.hd @@ List.tl data in

  let rec loop times dists =
    match times, dists with
    | [], [] -> 1
    | hdx :: tlx, hdy :: tly -> find_wins hdx hdy 0 * loop tlx tly
    | _ -> 1 in

  print_int @@ loop times dists
;;
