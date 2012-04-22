
let pint (x : option<int>)  =
  match x with
  | None -> ()
  | Some thing -> printfn "%d" thing

let bind (m : option<'a>)  (f : 'a -> option<'b>) : option<'b> =
  match m with
  | None -> None
  | Some x -> f x

let mul_3 x = 
  printfn "3x"
  Some (x * 3)

let mul_4 x = 
  printfn "4x"
  Some (x * 4)

let foo () =
  match (Some 2) with
  | None -> None
  | Some thing -> 
      match (mul_3 thing) with
      | None -> None
      | Some thing ->
        match (mul_4 thing) with
        | None -> None
        | Some thing -> Some thing


let bar () =
  bind (bind (Some 2) mul_3) (fun x -> mul_4 x)

foo () |> pint
bar () |> pint

