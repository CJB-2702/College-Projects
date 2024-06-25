Attribute VB_Name = "Module2"
Function DwgRevFromFolder(path As String)
    
    Sheets("Sheet1").UsedRange.Clear
    Dim oFSO As Object
    Dim oFolder As Object
    Dim oFile As Object
    
    Dim filename As String, dwg As String, rev As String
    
    Dim i As Integer
    
    Set oFSO = CreateObject("Scripting.FileSystemObject")
    
    If Len(path) < 1 Then
        'some arbitrary default path for demonstration reasons'
        path = "U:\CANES\_REQUEST FOR PROPOSAL\RFP 9065 LCC (HW2) AND VA (HW1.2)\VA TDP\CANES ENCLAVE SUBMARINE 0230055 VA CLASS TDP"
        Cells(2, 5).Value = "no path given in text box. Default folder  extracted"
    End If
    Cells(1, 1).Value = "File name"
    Cells(1, 2).Value = "Drawing #"
    Cells(1, 3).Value = "Rev"
    Cells(1, 4).Value = "Path"
    Cells(1, 5).Value = path
    
    
    Set oFolder = oFSO.GetFolder(path)
    i = 2
    
    For Each oFile In oFolder.Files
        filename = oFile.Name
        Cells(i, 1).Value = oFile.Name
        Cells(i, 2).Value = getdwg(filename)
        Cells(i, 3).Value = getrev(filename)
        
        'todo, change later used formula hack'
        
        Cells(i, 4).Value = path & "\" & oFile.Name
        i = i + 1
    Next oFile
End Function

