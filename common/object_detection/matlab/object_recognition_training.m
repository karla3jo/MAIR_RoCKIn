%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Claudi Ruiz
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all;
close all;
clc;

tic();

%% Variables a definir
reduccio=0.05;%0.001<reduccio<0.02 reducció de les imatges originals
conos=30; %Neurones pel NN 
pca=30; %nombre de features que volem del pca
numeros=5; % numero de classes
k=0; % contador de columnes per la variable input
mult=0; % quantitat de versions simulades de les mostres d'entrenament (multiple de quantitat de mostres d'entrenament)
        % util si no es tenen les suficients mesures
Sigma=0.5; % sigma pel filtre gaussia
talla=1;

%% Quantitat d'imatges per a cada classe
m1=1155;
m2=1074;
m3=1030;
m4=1000;
m5=826;

%% Assignem una certa quantitat del total de les 100 mostres originals per a entrenar i testejar (validar no ja que trainbr no fa validacio)
total=m1+m2+m3+m4+m5;
[trainInd,valInd,testInd] = dividerand(total,0.7,0.,0.3);
[trainIndsim,valIndsim,testIndsim] = dividerand(total*mult,0.7,0.,0.3);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% EXTRACCIO DE DADES I CREACIO DE LES FEATURES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
m=0;
for numero=1:numeros   
    clau=1;
    mostra = 1;
    while clau==1
        if numero == 1
            if     mostra < 10
                kase = ['cars\image_',num2str(0),num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 100
                kase = ['cars\image_',num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 1000
                kase = ['cars\image_',num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 10000
                kase = ['cars\image_',num2str(mostra),'.jpg']; 
            end        
        end
        
        if numero == 2
            if     mostra < 10
                kase = ['airplanes\',num2str(0),num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 100
                kase = ['airplanes\',num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 1000
                kase = ['airplanes\',num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 10000
                kase = ['airplanes\',num2str(mostra),'.jpg']; 
            end        
        end
        
        if numero == 3
            if     mostra < 10
                kase = ['guitars\',num2str(0),num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 100
                kase = ['guitars\',num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 1000
                kase = ['guitars\',num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 10000
                kase = ['guitars\',num2str(mostra),'.jpg']; 
            end        
        end
        
        if numero == 4
            if     mostra < 10
                kase = ['houses\',num2str(0),num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 100
                kase = ['houses\',num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 1000
                kase = ['houses\',num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 10000
                kase = ['houses\',num2str(mostra),'.jpg']; 
            end        
        end
        
        if numero == 5
            if     mostra < 10
                kase = ['motorbikes\',num2str(0),num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 100
                kase = ['motorbikes\',num2str(0),num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 1000
                kase = ['motorbikes\',num2str(0),num2str(mostra),'.jpg']; 
            elseif mostra < 10000
                kase = ['motorbikes\',num2str(mostra),'.jpg']; 
            end        
        end
        
        
        [I,map] = imread(kase);
        hola=I;
        original=I;
        I=imresize(I,[50 50],'bilinear');
        hola1=I;        
        %% Tractament imatges d'entrenament i test
        if size(I,3)==3
            I = rgb2gray(I); %Passem de color a grisos
        end
        I=normc(double(I));
        hola2=I;
        Mascara = fspecial('log',[3 3],Sigma);  %Creem la màscara Gaussiana
        I = imfilter(I,Mascara);  %Filtrem la imatge aplicant la màscara Gaussiana
        hola3=I;
        x=size(I,1);        
        y=size(I,2);
        diag=sqrt((x^2)+(y^2))*1.2;       
        black=double(zeros(round(diag),round(diag)));
        h1=round((diag-size(I,1))/2);
        h2=round((diag-size(I,2))/2);        
        black(h1:size(I,1)+h1-1,h2:size(I,2)+h2-1)=I;       
        original=I;
        I=black;
        k=k+1;
        %% Gradients
        hy = fspecial('sobel');
        hx = hy';
        gx = imfilter(I, hx); % x component of Sobel gradient
        gy = imfilter(I, hy); % y component
        Ix=times(gx,gx);
        Iy=times(gy,gy);
        I2=Ix+Iy;        
        input(:,k)=[I(:);I2(:)];
        inputinfo(:,k)=[numero;mostra;0];

        %% Simulacio de mostres d'entrenament i test
        if mult~=0 
        for z=1:mult
%             alpha=rand(1)*360;
            alpha=0;
            simI=imrotate(original,alpha,'loose');
            simblack=double(zeros(round(diag),round(diag)));
            h1=round((diag-size(simI,1))/2);
            h2=round((diag-size(simI,2))/2);
            simblack(h1:size(simI,1)+h1-1,h2:size(simI,2)+h2-1)=simI;
            
%             alea=round(1+2*rand(1));
%             if alea==3
%                 alea==4;
%             end
%             if alea==1
%             simblack=imresize(simblack,(alea*talla));
%             simblack=imresize(simblack,1/(alea*talla));
%             elseif alea==2
%             simblack=imresize(simblack,(alea*talla));
%             simblack=imresize(simblack,1/(alea*talla));
%             elseif alea==4
%             simblack=imresize(simblack,(alea*talla));
%             simblack=imresize(simblack,1/(alea*talla));
%             end
            hola4=simblack;
            
            %% Gradients
            hy = fspecial('sobel');
            hx = hy';
            gx = imfilter(simblack, hx); % x component of Sobel gradient
            gy = imfilter(simblack, hy); % y component
            simblackx=times(gx,gx);
            simblacky=times(gy,gy);
            simblack2=Ix+Iy;        
            siminput(:,k)=[simblack(:);simblack2(:)];
            siminputinfo(:,m+z)=[numero;mostra;alpha];                               
        end
        end
        m=m+mult;
        if     numero == 1
            if mostra == m1
                mostra=0;
                clau=0;
            end
        elseif numero == 2
            if mostra == m2
                mostra=0;
                clau=0;
            end
        elseif numero == 3
            if mostra == m3
                mostra=0;
                clau=0;
            end
        elseif numero == 4
            if mostra == m4
                mostra=0;
                clau=0;
            end
        elseif numero == 5
            if mostra == m5
                mostra=0;
                clau=0;
            end
        end
        mostra = mostra +1;
    end
%     if getappdata(h,'canceling')
%         break
%     end
%     percent=100*((numero*mostra)/(numeros*mostres))
%     waitbar(percent/100,h,sprintf('%12.1f',percent))
end
% delete(h);
features=size(I,1); % quantitat de caracteristiques

if mult==0
    siminput=[];
    siminputinfo=[];
    siminputinfo2=[];
    tsim=[];
    t1sim=[];
end

input=double(input);
siminput=double(siminput);
size(input)
size(siminput)

%% definim targets coneguts de les mesures d'entrenament
t=zeros(numeros,total);
t1=zeros(1,total);
close all
for i=1:total 
    
    if 1<=i && i<=m1
       t(:,i)=[1;0;0;0;0]; %classe 1
       t1(1,i)=0; 
    
    elseif m1<i && i<=m1+m2
       t(:,i)=[0;1;0;0;0]; %classe 2
       t1(1,i)=1;
       
    elseif m1+m2<i && i<=m1+m2+m3
       t(:,i)=[0;0;1;0;0]; %classe 3
       t1(1,i)=2; 
       
    elseif m1+m2+m3<i && i<=m1+m2+m3+m4
       t(:,i)=[0;0;0;1;0]; %classe 4
       t1(1,i)=3; 
       
    elseif m1+m2+m3+m4<i && i<=m1+m2+m3+m4+m5
       t(:,i)=[0;0;0;0;1]; %classe 5
       t1(1,i)=4; 
    end 
end

if mult~=0 
    for i=1:mult*total 

        if 1<=i && i<=(m1)*mult
            tsim(:,i)=[1;0;0;0;0]; 
            t1sim(1,i)=0; 
    
        elseif (m1)*mult<i && i<=(m1+m2)*mult
            tsim(:,i)=[0;1;0;0;0]; 
            t1sim(1,i)=1;
       
        elseif (m1+m2)*mult<i && i<=(m1+m2+m3)*mult
            tsim(:,i)=[0;0;1;0;0]; 
            t1sim(1,i)=2; 
       
        elseif (m1+m2+m3)*mult<i && i<=(m1+m2+m3+m4)*mult
            tsim(:,i)=[0;0;0;1;0]; 
            t1sim(1,i)=3; 
       
        elseif (m1+m2+m3+m4)*mult<i && i<=(m1+m2+m3+m4+m5)*mult
            tsim(:,i)=[0;0;0;0;1]; 
            t1sim(1,i)=4;        
        end 

    end
end

%% Seleccionem les mostres simulades d'entrenament amb els seus targets corresponents. 
ntrain=size([input(:,trainInd) siminput(:,trainIndsim)],2);
ntest=size([input(:,testInd) siminput(:,testIndsim)],2);
%% PCA, util per a obtenir una quantitat mes reduida de features pero alhora mes utils, per tal de facilitar la feina als classificadors
% PCA_projinput: Projection of the data A into the PCA space
[evec,PCA_projinput,eval] = princomp([input(:,trainInd) siminput(:,trainIndsim)]');
clear PCA_projinput;
% Check: Projection of data in the PCA basis is given by (A-mean(A))*evec:
projtrain =  ([input(:,trainInd) siminput(:,trainIndsim)]'-ones(ntrain,1)*mean([input(:,trainInd) siminput(:,trainIndsim)]'))*evec;
projtest  =  ([input(:,testInd) siminput(:,testIndsim)]'-ones(ntest,1)*mean([input(:,trainInd) siminput(:,trainIndsim)]'))*evec;
projtest2  =  [input(:,testInd) siminput(:,testIndsim)]';

projtest2info = [inputinfo(:,testInd) siminputinfo(:,testIndsim)]';
inputtrain=projtrain(:,1:pca)'; %Inputs d'entrenament post PCA
inputtest=projtest(:,1:pca)'; % Inputs de testeig post PCA
clear projtrain;
figure();
plot(cumsum(100*eval/sum(eval)),'ro') % Quantitat d'informaciï¿½ que ens proporcionen les dimensions del PCA, mes informacio no implica millors resultats
xlabel('PCA dimensions')
ylabel('Information percentage')
axis([0 500 0 100]);
clear eval;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CLASSIFICADOR
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Preparem algunes variables pels classificadors
intervals=zeros(2,features);
N = size(inputtest',1);

% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% NEURAL NETWORK
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
display('%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%')
display('NEURAL NETWORK')
net = patternnet(conos);
[net,tr] = trainbr(net,inputtrain,[t(:,trainInd) tsim(:,trainIndsim)]);
%%%nntraintool
figure
plotconfusion([t(:,trainInd) tsim(:,trainIndsim)],net(inputtrain));
title('Train')
%  print -depsc -tiff -r300 confusiontrain
figure
plotconfusion([t(:,testInd) tsim(:,testIndsim)],net(inputtest))
title('Test')
trains = [t(:,trainInd) tsim(:,trainIndsim)];
trains2= round(net(inputtrain));
successtrain=0;
for j=1:size(trains,2)
    successtrain=successtrain+isequal(trains(:,j),trains2(:,j));
end
successtrain=100*successtrain/(size(trains,2));
errortrainVTrain(1,gr)=100-successtrain;

tests = [t(:,testInd) tsim(:,testIndsim)];
tests2= round(net(inputtest));
successtest=0;
for j=1:size(tests,2)
    successtest=successtest+isequal(tests(:,j),tests2(:,j));
end
successtest=100*successtest/(size(tests,2));
errortestVTest(1,gr)=100-successtest;

toc();
