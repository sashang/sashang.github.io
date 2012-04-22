open System

type RequestState = { 
  request : Net.WebRequest; 
  url: string 
}

let create_thread request url =  
  let helper request url =    
    let tid = string (Threading.Thread.CurrentThread.ManagedThreadId)
    request url    

  let t = new Threading.Thread(Threading.ThreadStart(fun _ -> helper request url))
  t

let request_url (url : string) =
  let tid = string (Threading.Thread.CurrentThread.ManagedThreadId)
  Console.WriteLine ("Thread id = " + tid + ", Request for " + url)  
  let wr = Net.WebRequest.Create url  
  use response = wr.GetResponse()  
  Console.WriteLine ("Thread id = " + tid + ", Response for " + url)  

let get_response_cb (result : IAsyncResult) =
  let tid = string(Threading.Thread.CurrentThread.ManagedThreadId)  
  let rs = result.AsyncState :?> RequestState
  use response = rs.request.EndGetResponse(result)  
  Console.WriteLine ("Thread id = " + tid + ", Response for " + rs.url)    
  use stream = response.GetResponseStream()
  use reader = new System.IO.StreamReader(stream)
  let html = reader.ReadToEnd()
  ()
  
let begin_get_response (url : string) =
  let wr = Net.WebRequest.Create url
  let cb = new AsyncCallback(get_response_cb)
  let result = wr.BeginGetResponse(cb, {request =  wr; url = url})
  let tid = string (Threading.Thread.CurrentThread.ManagedThreadId)
  Console.WriteLine ("Thread id = " + tid + ", Request for " + url)
  ()

///run the single threaded synchronous calls
let ss_run urls =
  Console.WriteLine "Single-threaded synchronous requests ..."
  List.iter request_url urls 

///run the multi-threaded synchronous version
let ms_run urls =
  Console.WriteLine "Multi-threaded synchronous requests ..."
  let tasks = List.map (fun url -> create_thread request_url url) urls  
  List.iter (fun (thread : Threading.Thread) -> thread.Start()) tasks
  //wait for all threads to complete  
  List.iter (fun (thread : Threading.Thread) -> thread.Join()) tasks

let st_request (url : string) max_urls (count : int ref) 
  (finished : Threading.ManualResetEvent) =  
  async {    
    let tid = string(Threading.Thread.CurrentThread.ManagedThreadId)  
    let wr = Net.WebRequest.Create url
    Console.WriteLine("Thread id = " + tid + ", Request for " + url)
    use! response = wr.AsyncGetResponse()    
    count := !count + 1
    let tid = string(Threading.Thread.CurrentThread.ManagedThreadId)
    Console.WriteLine("Thread id = " + tid + ", Response for " + url)    
    if !count = 6 then 
      finished.Set() |> ignore
    ()
  }

let mt_request (url : string) max_urls (count : int ref) 
  (finished : Threading.ManualResetEvent) =  
  async {
    let tid = string(Threading.Thread.CurrentThread.ManagedThreadId)  
    let wr = Net.WebRequest.Create url
    Console.WriteLine("Thread id = " + tid + ", Request for " + url)
    use! response = wr.AsyncGetResponse()
    Threading.Interlocked.Increment(count) |> ignore
    let tid = string(Threading.Thread.CurrentThread.ManagedThreadId)  
    Console.WriteLine("Thread id = " + tid + ", Response for " + url)    
    if !count = 6 then 
      finished.Set() |> ignore
    ()
  }

///run the single threaded asynchronous version
let sa_run =
  let finished = new Threading.ManualResetEvent(false)
  let count = ref 0
  let ctxt = Threading.SynchronizationContext.Current 
  (fun (urls : list<string>) ->
    Console.WriteLine("Single-threaded asynchronous requests ...")
    List.iter (fun url -> 
      Async.StartImmediate(st_request url urls.Length count finished)) urls
    finished.WaitOne() |> ignore)

let ma_run =
  let count = ref 0
  let finished = new Threading.ManualResetEvent(false)
  (fun (urls : list<string>) ->
    Console.WriteLine("Multi-threaded asynchronous requests ... ")
    List.iter (fun url ->
      Async.Start(mt_request url urls.Length count finished)) urls
    finished.WaitOne() |> ignore)

let main () =
  let urls = ["http://www.google.com"; "http://www.microsoft.com"; 
    "http://www.yahoo.com"; "http://www.wordpress.com"; 
    "http://www.blizzard.com"; "http://www.valvesoftware.com"]
  ss_run urls
  ms_run urls
  sa_run urls
  ma_run urls
  Console.WriteLine "Press any key to end ..."
  Console.ReadLine() |> ignore

main ()
