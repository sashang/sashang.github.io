#!/bin/env -S dotnet fsi

open System.Net.Http

let get() =
    use client = new HttpClient()
    let uri = "http://www.contoso.com"
    let response = client.GetAsync(uri)
    printfn "%A" response

get()

