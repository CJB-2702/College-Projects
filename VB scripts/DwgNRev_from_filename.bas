Attribute VB_Name = "Module1"
Function Remove_Spaces(string1 As String) As String
    
    'Variable Declaration'
    Dim TempString As String
    Dim x As Integer
    'Remove Extra Spaces'
    Do
        TempString = string1
        x = InStr(filename, " ")
        string1 = Replace(string1, " ", "")
    Loop Until TempString = string1
        
   Remove_Spaces = string1
    
End Function

Function getdwg(fn As String) As String
    fn = UCase(fn)
    
    Dim x As Integer
    
    x = InStr(fn, "REV")
    If x > 0 Then
        getdwg = "'" & Left(fn, x - 2)
    Else
        x = InStr(fn, " ")
        If x > 1 Then
            getdwg = "'" & Left(fn, x - 1)
        Else
            getdwg = ""
        End If
    End If
End Function

Function getrev(fn As String) As String
    fn = UCase(fn)
    Dim x As Integer
    x = i
    x = InStr(fn, "REV")
    If x > 0 Then
        fn = Remove_Spaces(fn)
        getrev = Mid(fn, x + 3, 1)
        
        If getrev = "X" Then
        getrev = "'" & Mid(fn, x + 3, 2)
        'possible inclusion of x10+ in future'
        End If
        
    Else
        x = InStr(fn, " ")
        If x > 1 Then
            getrev = "'" & Mid(fn, x + 1, 1)
            If getrev = "'X" Then
                getrev = "'" & Mid(fn, x + 1, 2)
            End If
        Else
            getrev = ""
        End If
    End If
End Function

