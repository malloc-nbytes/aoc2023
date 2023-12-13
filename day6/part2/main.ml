let file_to_str filename =
  let ch = open_in_bin filename in
  let s = really_input_string ch (in_channel_length ch) in
  close_in ch;
  s
;;

let rec find_wins time dist i acc =
  match i > time with
  | true -> acc
  | _ ->
     let our_dist = i*(time-i) in
     if our_dist > dist then
       find_wins (time) dist (i+1) (acc+1)
     else
       find_wins (time) dist (i+1) acc

let filepath = "./input.txt"
let () =
  let times = [51699878]
  and dists = [377117112241505] in

  let rec loop times dists =
    match times, dists with
    | [], [] -> 1
    | hdx :: tlx, hdy :: tly -> find_wins hdx hdy 0 0 * loop tlx tly
    | _ -> 1 in

  print_int @@ loop times dists
;;
