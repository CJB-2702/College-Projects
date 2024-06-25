Attribute VB_Name = "Module1"
Function getHigherRev(C1 As String, C2 As String) As String

    If C1 = "" Or C2 = "" Then
        ''' added because of bug'''
        If C1 = "" Then
            C2 = C1
        End If
        C2 = C1
    End If
    
    
    If C1 = C2 Then
        getHigherRev = C1
    Else
        If Left(C1, 1) = "X" Or Left(C2, 1) = "X" Then
            If Left(C1, 1) = "X" Then
                If Left(C2, 1) = "X" Then
                    getHigherRev = IIf(C2 > C1, C2, C1)
                Else
                          getHigherRev = C2
                End If
            Else
                ''' right is x and left is not'''
                getHigherRev = C1
            End If
        Else
            ''C1 and C2 are normal "-" through Z characters''
            getHigherRev = IIf(C2 > C1, C2, C1)
        End If
    End If
End Function



Function HighestRevXlookup(dwg As String, inCol As Range)
    Dim ismore As Boolean
    Dim pos As Range
    Do
     
    While ismore = False
        
End Function
