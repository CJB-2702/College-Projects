Attribute VB_Name = "Module1"
Function LastPosition(rCell As Range, rChar As String)
    'This function gives the last position of the specified character
    'This code has been developed by Sumit Bansal (https://trumpexcel.com)
    Dim rLen As Integer
    rLen = Len(rCell)
    For i = rLen To 1 Step -1
        If Mid(rCell, i - 1, 1) = rChar Then
            LastPosition = i
            Exit Function
        End If
    Next i
End Function


Function trimend(rCell As Range, rChar As String) As String
    trimend = Left(rCell, LastPosition(rCell, rChar) - 2)
End Function


