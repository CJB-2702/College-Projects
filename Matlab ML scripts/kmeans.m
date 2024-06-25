%{
CHRISTIAN BISSETT

ECE 174, INSTRUCTOR PIYA PAL
PROGRAM 2, K MEANS CLUSTERING
%}
%{
DIRECTIONS ----------------------------------------------------------------
    Write a program k-means to implement k-means clustering.

    The functions accepts two inputs:
    a list of N data vectors to be clustered, (x1..xn)
    and the number of clusters,K.

    The program outputs three quantities:
    a list of N group assignment indices(c1, c2,    , cN)
    a list of K group representative vectors (z1,    , zK)
    and the value of Jclust after each iteration of the algorithm until convergence or termination.
%}
%{
NOTES ----------------------------------------------------------------------
THIS SCRIPT ONLY OPERATES IF THE MATLAB FUNCTIONS
regroup()          generateZ()
ARE SAVED IN THE SAME FOLDER,WITH THEIR FILE NAMES MATCHING THEIR FUNCTION NAMES.
%}
function [c, z ,Jclust] = kmeans(x,k,maxits)

    %Initialization:-------------------------------------------------------
    %Initialize k-means by randomly assigning the data points to K group
    %i.e., randomly selecting each ci to be an integer between 1 and K.

    % "rows"= N images...  "columns"= 784 pixels 
    [rowsx,colsx] = size(x);
    c = randi(k,rowsx,1); %Initialize k-means by randomly assigning the data points

    %I am going to take a leap of faith here and assume all K groups will
    %have at least one Ci entry minimum

    %specifying cluster assignments----------------------------------------
    %Let Gj be the set of indices corresponding to group j
    %ex: if N=5 vectors were clustered into 3 groups randomally assigned a
    %map c=(3,1,1,1,2) G={2,3,4; 5; 1}

    G = zeros(k,1);
    GjCount=zeros(k,1); % an array that keeps track of how big each k vector is

    for iterator = 1:rowsx
        groupNumber=c(iterator);
        GjCount(groupNumber)= 1+GjCount(groupNumber);%group size is now one bigger
        G(groupNumber,GjCount(groupNumber))= iterator; %loading adress into Gij
    end

    %{
    Group representatives ----------------------------------------------
    we want the representatives to be close to the clustered values as
    possible, however because they are all random the "closest value" would
    just put all the starting points right at the center
    1) they are all random so theoretically a random selection is just as
    good as anything else
    %}    
    z=zeros(k,colsx);
    for iterator= 1:k
        randomIndexInClusterJ=G(randi(GjCount(iterator)));
        z(iterator,:)= x(randomIndexInClusterJ,:);
    end


    %======================================================================
    %ACTUALLY STARTING THE K MEANS ALGORITHEM
    %======================================================================
    %given a list of N vectors, X and K group representative vectors repeat
    %until convergence

    %partition the vectors into K groups

    %update representatives

    maxIterationsAllowed=maxits;
    N_its=0;
    Jclust=zeros(3,1);
    isConverged=false;
    
    groupnumber=0;
    while isConverged==false
        N_its=N_its+1;
        if N_its>maxIterationsAllowed
            break
        end

        %partition the vectors into the different groups-------------------

        %clearing groups
        G = zeros(k,1);
        GjCount=zeros(k,1);

        %regrouping
        [Ngroups,~]=size(z);
        normsXZ=zeros(Ngroups,1);
        for itX = 1:rowsx
            for itZ= 1:Ngroups
                temp=x(itX,:)- z(itZ,:);
                normsXZ(itZ)=norm(temp);
            end
            %[value,index]= min(...)
            [~,groupnumber]= min(normsXZ);
            GjCount(groupnumber)=GjCount(groupnumber)+1;
            G(groupnumber,GjCount(groupnumber))= itX;
            c(itX)=groupnumber;
        end

        %update representatives------------------------------------------------
        
        %{ 
        %this code was really effective at grouping them but would leave
        %odd numbers of results and hurt my ability to write the paper
        %delete empty groups, adjust variables
        for it1=1:Ngroups
            if GjCount(it1)==0
                %shift all the group numbers that are higher,down one
                isShift=c>it1;
                c=c-isShift;
            end
        end
        Ngroups=max(c);
        %}
        
        z=zeros(Ngroups,colsx);
        %sum up all the images in each group
        for it=1:rowsx
            groupnumber=c(it,1);
            z(groupnumber,:)=z(groupnumber,:)+ x(it,:);
        end
        %divide by the number in group
        for it=1:Ngroups
            isInGroup= c==it;
            GjCount(it)= dot(isInGroup,isInGroup);%array of ones , dot gives count
            z(it,:)=z(it,:)/GjCount(it);
        end
        
        

        %Quantify optimisation-------------------------------------------------
        JcxN=0;
        %summing norms
        for it1=1:Ngroups
            xi_zj=x(it1,:)-z(c(it1),:);
            JcxN=JcxN+dot(xi_zj,xi_zj);
        end
        %turning sum into square mean
        Jclust(N_its)=JcxN/rowsx;




        %putting in convergence criterion------------------------------------
        %{
        %ended up taking this out because the max its was just way more
        %effective than this.
        if (N_its==3)
            deltaJcInit=Jclust(1)-Jclust(3);
        end
        if(N_its>3)
            if(Jclust(N_its-1)-Jclust(N_its)< deltaJcInit/10000)
                isConverged=true;
            end
        end
        if zold==z
            isConverged=true;
        end
        %}
        
    end %endwhile
    
    fprintf('Kmeans\n');
end