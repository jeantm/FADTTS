%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%       FADTTS Analysis       %%%%%%%%%%%%
%%%%%%%%%%%% FADTTS Executable $version$ %%%%%%%%%%%%
%%%%%%%%%%%%    $date$ at $time$   %%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all
clear all
clc
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%% Inputs %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%% Folder %%%
[ folder, name, ext ] = fileparts( mfilename( 'fullpath' ) );

%%% Input FiberName %%%
$inputFiberName$
%%% Input  Diffusion Properties %%%
$inputAllProperties$
$inputDiffusionProperties$
%%% Input Files %%%
$inputMatlabCOMPInputFile$
$inputDiffusionFiles$
%%% Input Covariates %%%
$inputNbrCovariates$
$inputCovariates$
%%% Input Running Options %%%
$inputNbrPermutations$
$inputOmnibus$
$inputPostHoc$



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%% Script %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

addpath '/NIRAL/devel/linux/FADTTS/FADTTS_V3.01_NIRAL/FADTTS/FADTTS/';

%COLORS
orange =[1 0.5 0.2];
gray =[0.5 0.5 0.5];
purple =[0.4 0 0.5];
darkgreen =[0 0.65 0];
mustard =[0.8 0.75 0];
turquoise =[0 0.75 0.75];
darkred =[0.65 0 0];
blue =[0 0 1];
magenta =[1 0 1];
cyan = [0 1 1];
red = [1 0 0];

% Fiber Names
Fnames = cell( 1, 1 );
Fnames{ 1 } = fiberName;

% Diffusion Properties
$diffusionProperties$
% Diffusion Properties Tested for file names
params = cell( 1, 1 );
$allProperties$

% number of bootstrapping permutations to use in Hypothesis testing
numPerms = nbrPermutations; % Use 100 when testing scripts. Use 1000 when running scripts for real.


%% COVARIATES
% this file has all covariates organized in columns (without headings):

$matlabCOMPInputFile$
designdata = [ ones( size( data2, 1 ), 1 ) data2 ]; % intercept + covariates

P = nbrCovariates; % how many covariates do you have? (including intercept)

Pnames = cell( P, 1 );
$covariates$

%% LOAD FIBER DATA
% diffusionFiles should be a (arclength)X(subject) matrix.
% the first column has arc-lengths

$diffusionFiles$

%% ARCLENGTH
% reading (x,y,z) coordinates

% Get arclength from input file
arclength = dataFiber1All( :, 1 ); % take first column => arclength from dtiCC_statCLP fiber file

% Creating (x,y,z) coordinates
CC_data = [ arclength zeros( size( arclength, 1 ), 1 ) zeros( size( arclength, 1 ), 1 ) ];

nofeatures = size( diffusionFiles, 1 );
[ NoSetup, arclength_allPos, Xdesign, Ydesign ] = MVCM_read( CC_data, designdata, diffusionFiles, nofeatures );
N = NoSetup( 1 );	% No of subjects
A = NoSetup( 2 ); % No of arclengths
P = NoSetup( 3 ); % No of covariates (including intercept)
D = NoSetup( 4 );	% No of diffusion properties = 1




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% PLOT CC_Genu PARAMETER - raw data.
% Can color by categorical variable of your choice.

figure();
pii= 9; %  TwinStatus
for Dii=1:4
    for nii=1:N % subjects
        if (designdata(nii,pii) == 0)
            h(1)=plot(arclength,Ydesign(nii,:,Dii),'-k','LineWidth', 2);
        else
            h(2)=plot(arclength,Ydesign(nii,:,Dii),'-c','LineWidth', 2);
        end
        hold on;
    end
    hold off;
    xlabel('arclength');
    ylabel(Dnames{Dii});
    xlim([min(arclength) max(arclength)]);
    title(sprintf('%s %s',Fnames{1},Dnames{Dii}));
    legend(h,sprintf('%s=0',Pnames{pii}),sprintf('%s=1',Pnames{pii}),'Location','SouthEastOutside');
    clear h;
    
    % save plot
    figurename=sprintf('%s_%s_group.pdf',Fnames{1},Dnames{Dii});
    saveas(gcf,figurename,'pdf');
end
close();

%% PLOT CC_Genu PARAMETER - raw data.
% Can color by categorical variable of your choice.

figure();
pii= 3; %  variables: sex
for Dii=1:4
    for nii=1:N % subjects
        if (designdata(nii,pii) == 1)
            h(1)=plot(arclength,Ydesign(nii,:,Dii),'-b','LineWidth', 2);
        else
            h(2)=plot(arclength,Ydesign(nii,:,Dii),'-r','LineWidth', 2);
        end
        hold on;
    end
    hold off;
    xlabel('arclength');
    ylabel(Dnames{Dii});
    xlim([min(arclength) max(arclength)]);
    title(sprintf('%s %s',Fnames{1},Dnames{Dii}));
    legend(h,sprintf('%s=0',Pnames{pii}),sprintf('%s=1',Pnames{pii}),'Location','SouthEastOutside');
    clear h;
    
    % save plot
    figurename=sprintf('%s_%s_Sex.pdf',Fnames{1},Dnames{Dii});
    saveas(gcf,figurename,'pdf');
end
close();

%% Plot Raw Data Average and Standard Deviation-
% Can plot categorical variable of your choice.

pii= 9;  % TwinStatus
[Mavg]= mean(Ydesign(designdata(:,pii)==0,:,:)); % TD average for each diffusion paramter
[Mstddev]= std(Ydesign(designdata(:,pii)==0,:,:)); % TD standard deviation for each diffusion paramter
[Favg]= mean(Ydesign(designdata(:,pii)==1,:,:)); % SE average for each diffusion paramter
[Fstddev]= std(Ydesign(designdata(:,pii)==1,:,:)); % SE standard deviation for each diffusion paramter
for Dii=1:4
    figure();
    h(1)=plot(arclength, Mavg(:,:,Dii),'-k','LineWidth', 2);
    hold on;
    plot(arclength, Mavg(:,:,Dii)+Mstddev(:,:,Dii),'--k','LineWidth',2);
    hold on;
    plot(arclength, Mavg(:,:,Dii)-Mstddev(:,:,Dii),'--k','LineWidth',2);
    hold on;
    
    h(2)=plot(arclength, Favg(:,:,Dii),'-c','LineWidth', 2);
    hold on;
    plot(arclength, Favg(:,:,Dii)+Fstddev(:,:,Dii),'--c','LineWidth',2);
    hold on;
    plot(arclength, Favg(:,:,Dii)-Fstddev(:,:,Dii),'--c','LineWidth',2);
    hold off;
    
    xlabel('arclength');
    ylabel(Dnames{Dii});
    xlim([min(arclength) max(arclength)]);
    title(sprintf('%s %s Average and Standard Deviation',Fnames{1},Dnames{Dii}));
    legend(h,sprintf('%s=0',Pnames{pii}),sprintf('%s=1',Pnames{pii}),'Location','SouthEastOutside');
    clear h;
    
    % save plot
    figurename=sprintf('%s_%s_Avg_StdDev_Group.pdf',Fnames{1},Dnames{Dii});
    saveas(gcf,figurename,'pdf');
    close();
end
%% Plot Raw Data Average and Standard Deviation-
% Can plot categorical variable of your choice.

pii= 3; % Sex
[Mavg]= mean(Ydesign(designdata(:,pii)==0,:,:)); % Male average for each diffusion paramter- the value must match in subject matrix
[Mstddev]= std(Ydesign(designdata(:,pii)==0,:,:)); % Male standard deviation for each diffusion paramter
[Favg]= mean(Ydesign(designdata(:,pii)==1,:,:)); % Female average for each diffusion paramter
[Fstddev]= std(Ydesign(designdata(:,pii)==1,:,:)); % Female standard deviation for each diffusion paramter
for Dii=1:4
    figure();
    h(1)=plot(arclength, Mavg(:,:,Dii),'-b','LineWidth', 2);
    hold on;
    plot(arclength, Mavg(:,:,Dii)+Mstddev(:,:,Dii),'--b','LineWidth',2);
    hold on;
    plot(arclength, Mavg(:,:,Dii)-Mstddev(:,:,Dii),'--b','LineWidth',2);
    hold on;
    
    h(2)=plot(arclength, Favg(:,:,Dii),'-r','LineWidth', 2);
    hold on;
    plot(arclength, Favg(:,:,Dii)+Fstddev(:,:,Dii),'--r','LineWidth',2);
    hold on;
    plot(arclength, Favg(:,:,Dii)-Fstddev(:,:,Dii),'--r','LineWidth',2);
    hold off;
    
    xlabel('arclength');
    ylabel(Dnames{Dii});
    xlim([min(arclength) max(arclength)]);
    title(sprintf('%s %s Average and Standard Deviation',Fnames{1},Dnames{Dii}));
    legend(h,sprintf('%s=0',Pnames{pii}),sprintf('%s=1',Pnames{pii}),'Location','SouthEastOutside');
    clear h;
    
    % save plot
    figurename=sprintf('%s_%s_Avg_StdDev_Sex.pdf',Fnames{1},Dnames{Dii});
    saveas(gcf,figurename,'pdf');
    close();
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




%% 2. fit a model using local polynomial kernel smoothing
% calculating Betas

[ mh ] = MVCM_lpks_wob( NoSetup, arclength_allPos, Xdesign, Ydesign );
[ efitBetas, efitBetas1, InvSigmats, efitYdesign ] = MVCM_lpks_wb1( NoSetup, arclength_allPos, Xdesign, Ydesign, mh );




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% plot betas  - old way of plotting betas - zoomed out with intercept
% betas are the coefficients that describe how related the covariate is to
% the parameter
for Dii=1:4
    figure();
    h=plot(arclength,efitBetas(1,:,Dii),'-k','LineWidth', 2); %Intercept
    hold on;
    h=plot(arclength,efitBetas(2,:,Dii),'Color',red,'LineWidth', 2); %Composite
    hold on;
    h=plot(arclength,efitBetas(3,:,Dii),'Color',purple,'LineWidth', 2); %Sex
    hold on;
    h=plot(arclength,efitBetas(4,:,Dii),'Color',cyan,'LineWidth', 2); %GestAgeBirth
    hold on;
    h=plot(arclength,efitBetas(5,:,Dii),'Color',blue,'LineWidth', 2); %DaysSinceBirth
    hold on;
    h=plot(arclength,efitBetas(6,:,Dii),'Color',mustard,'LineWidth', 2); %Maternal Education
    hold on;
    h=plot(arclength,efitBetas(7,:,Dii),'Color',orange,'LineWidth', 2); %Scanner
    hold on;
    h=plot(arclength,efitBetas(8,:,Dii),'Color',darkred,'LineWidth', 2); %Directions
    hold on;
    h=plot(arclength,efitBetas(9,:,Dii),'Color',darkgreen,'LineWidth', 2); %TwinStatus
    hold on;
    h=plot(arclength,efitBetas(10,:,Dii),'Color',turquoise,'LineWidth', 2); %ICV
    hold off;
    xlabel('arclength');
    ylabel(Dnames{Dii});
    title(sprintf('%s %s Estimated Coefficients',Fnames{1},Dnames{Dii}));
    xlim([min(arclength) max(arclength)]);
    h=legend(Pnames(1:P),'Location','SouthEastOutside');
    clear h;
    
    % save plot
    figurename=sprintf('%s_%s_Old_betas.pdf',Fnames{1},Dnames{Dii});
    saveas(gcf,figurename,'pdf');
    
    % save Beta txt file
    savefile=sprintf('%s_%s_betas.csv',Fnames{1},Dnames{Dii});
    temp=efitBetas(:,:,Dii);
    csvwrite(savefile,temp);
    clear temp;
    close();
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




%% smoothing individual function

ResYdesign = Ydesign - efitYdesign;
[ ResEtas, efitEtas, eSigEta ] = MVCM_sif( arclength_allPos, ResYdesign );
[ mSigEtaEig, mSigEta ] = MVCM_eigen( efitEtas );




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% plot eigenvalue : first 12 eigenvalues

figure();
plot(mSigEtaEig(1:12,1,1),'-r.','LineWidth', 2);  % FA
hold on;
plot(mSigEtaEig(1:12,1,2),'-g.','LineWidth', 2);  % MD
hold on;
plot(mSigEtaEig(1:12,1,3),'-b.','LineWidth', 2);  % RD
hold on;
plot(mSigEtaEig(1:12,1,4),'-m.','LineWidth', 2);  % AD
hold off;
xlim([1 12]);
legend([Dnames(1:D)],'Location','SouthEastOutside');
title(sprintf('%s Eigenvalues',Fnames{1}));
xlabel('component');
ylabel('variance');

% save plot
figurename=sprintf('%s_%s_eigenvalues.pdf',Fnames{1},params{myparams});
saveas(gcf,figurename,'pdf');
close();

%% plot eigenvector : first 3 eigenvectors
for Dii=1:4
    figure();
    plot(arclength,mSigEtaEig(1,2:end,Dii),'-r.','LineWidth', 2); % 1st eigenvector
    hold on;
    plot(arclength,mSigEtaEig(2,2:end,Dii),'-g.','LineWidth', 2); % 2nd eigenvector
    hold on;
    plot(arclength,mSigEtaEig(3,2:end,Dii),'-b.','LineWidth', 2); % 3rd eigenvector
    hold off;
    h=legend('1st eigenvector','2nd eigenvector','3rd eigenvector','Location','SouthEastOutside');
    xlabel('arclength');
    ylabel(Dnames{Dii});
    xlim([min(arclength) max(arclength)]);
    clear h;
    title(sprintf('%s %s Eigenvectors',Fnames{1},Dnames{Dii}));
    figurename=sprintf('%s_%s_eigenvectors.pdf',Fnames{1},Dnames{Dii});
    
    % save plot
    saveas(gcf,figurename,'pdf');
    close();
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




%% 3. Omnibus Hypothesis Test
if( omnibus == 1 )
    Gstats = zeros( 1, P-1 );
    Lstats = zeros( A, P-1 );
    Gpvals = zeros( 1, P-1 );
    
    [ ebiasBetas ] = MVCM_bias( NoSetup, arclength_allPos, Xdesign, Ydesign, InvSigmats, mh );
    
    for pp=2:P
        %individual and global statistics calculation
        cdesign=zeros( 1, P );
        cdesign( pp ) = 1;
        Cdesign = kron( eye( D ), cdesign );
        B0vector = zeros( D, A );
        [Gstat, Lstat] = MVCM_ht_stat( NoSetup, arclength_allPos, Xdesign, efitBetas, eSigEta, Cdesign, B0vector, ebiasBetas );
        Gstats( 1, pp-1 ) = Gstat;
        Lstats( :, pp-1 ) = Lstat;
        
        % Generate random samples and calculate the corresponding statistics and pvalues
        GG = numPerms;
        [Gpval] = MVCM_bstrp_pvalue3( NoSetup, arclength_allPos, Xdesign, Ydesign, efitBetas1, InvSigmats, mh, Cdesign, B0vector, Gstat, GG );
        Gpvals( 1, pp-1 ) = Gpval;
    end
    
    Lpvals = 1-chi2cdf( Lstats, D );
    
    
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% plot local p values  - These p-values are NOT corrected for multiple comparisons
    % need a plot line for each covariate
    
    figure();
    h=plot(arclength,-log10(Lpvals(:,1)),'Color',red,'LineWidth', 2,'MarkerSize',15); %Composite
    hold on;
    h=plot(arclength,-log10(Lpvals(:,2)),'Color',purple,'LineWidth', 2,'MarkerSize',15); %Sex
    hold on;
    h=plot(arclength,-log10(Lpvals(:,3)),'Color',cyan,'LineWidth', 2,'MarkerSize',15); %GestAgeBirth
    hold on;
    h=plot(arclength,-log10(Lpvals(:,4)),'Color',blue,'LineWidth', 2,'MarkerSize',15); %DaysSinceBirth
    hold on;
    h=plot(arclength,-log10(Lpvals(:,5)),'Color',mustard,'LineWidth', 2,'MarkerSize',15); %MEDUC
    hold on;
    h=plot(arclength,-log10(Lpvals(:,6)),'Color',orange,'LineWidth', 2,'MarkerSize',15); %Scanner
    hold on;
    h=plot(arclength,-log10(Lpvals(:,7)),'Color',darkred,'LineWidth', 2,'MarkerSize',15); %Directions
    hold on;
    h=plot(arclength,-log10(Lpvals(:,8)),'Color',darkgreen,'LineWidth', 2,'MarkerSize',15); %TwinStatus
    hold on;
    h=plot(arclength,-log10(Lpvals(:,9)),'Color',turquoise,'LineWidth', 2,'MarkerSize',15); %ICV
    hold off;
    
    xlabel('arclength');
    xlim([min(arclength) max(arclength)]);
    ylabel('-log10(p)');
    xL = get(gca,'XLim');
    h=legend(Pnames(2:P),'Location','SouthEastOutside');
    line(xL,[1.3 1.3],'Color','black'); % line at 1.3 to mark significance level
    clear h;
    title(sprintf('%s %s Local p-values',Fnames{1},params{myparams}));
    
    %save plot
    saveas(gcf,sprintf('%s_%s_Local_pvalues.pdf',Fnames{1},params{myparams}),'pdf');
    close();
    %save Local P-Values csv file
    csvwrite(sprintf('%s_%s_Local_pvalues.csv',Fnames{1},params{myparams}),Lpvals);  % column for each covariate; local p-values are computed at each arclength
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
    
    
    %% Global p-value
    
    Gpvals
    
    csvwrite( sprintf( '%s/%s_%s_Global_pvalues.csv', folder, Fnames{1}, params{1} ), Gpvals ); %save csv file
    
    %% correct local p-values with FDR
    % this corrects the local p-values for multiple comparisons
    
    Lpvals_FDR = zeros( size( Lpvals ) );
    for i = 1:( P-1 )
        Lpvals_FDR( :, i ) = mafdr( Lpvals( :, i ), 'BHFDR', true );
    end
    
    % save FDR Local P-Values csv file
    csvwrite( sprintf( '%s/%s_%s_FDR_Local_pvalues.csv', folder, Fnames{1}, params{1} ), Lpvals_FDR );
    
    
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% plot corrected local p values for each covariate
    % need a plot line for each covariate
    
    figure();
    h=plot(arclength,-log10(Lpvals_FDR(:,1)),'Color',red,'LineWidth', 2,'MarkerSize',15); %Composite
    hold on;
    h=plot(arclength,-log10(Lpvals_FDR(:,2)),'Color',purple,'LineWidth', 2,'MarkerSize',15); %Gender
    hold on;
    h=plot(arclength,-log10(Lpvals_FDR(:,3)),'Color',cyan,'LineWidth', 2,'MarkerSize',15); %GestAgeBirth
    hold on;
    h=plot(arclength,-log10(Lpvals_FDR(:,4)),'Color',blue,'LineWidth', 2,'MarkerSize',15); %DaysSinceBirth
    hold on;
    h=plot(arclength,-log10(Lpvals_FDR(:,5)),'Color',mustard,'LineWidth', 2,'MarkerSize',15); %MEDUC
    hold on;
    h=plot(arclength,-log10(Lpvals_FDR(:,6)),'Color',orange,'LineWidth', 2,'MarkerSize',15); %Scanner
    hold on;
    h=plot(arclength,-log10(Lpvals_FDR(:,7)),'Color',darkred,'LineWidth', 2,'MarkerSize',15); %Directions
    hold on;
    h=plot(arclength,-log10(Lpvals_FDR(:,8)),'Color',darkgreen,'LineWidth', 2,'MarkerSize',15); %TwinStatus
    hold on;
    h=plot(arclength,-log10(Lpvals_FDR(:,9)),'Color',turquoise,'LineWidth', 2,'MarkerSize',15); %ICV
    hold off;
    
    xlabel('arclength');
    xlim([min(arclength) max(arclength)]);
    ylabel('-log10(p)');
    xL = get(gca,'XLim');
    h=legend(Pnames(2:P),'Location','SouthEastOutside');
    line(xL,[1.3 1.3],'Color','black'); % line at 1.3 to mark significance level
    clear h;
    title(sprintf('%s %s Corrected Local p-values',Fnames{1},params{myparams}));
    
    %save plot
    saveas(gcf,sprintf('%s_%s_Corrected_Local_pvalues.pdf',Fnames{1},params{myparams}),'pdf');
    close();
    
    %% plot corrected local p values for just GROUP
    
    figure();
    plot(arclength,-log10(Lpvals_FDR(:,1)),'-r.','LineWidth', 2,'MarkerSize',15); % EL
    xlabel('arclength');
    xlim([min(arclength) max(arclength)]);
    ylabel('-log10(p)');
    xL = get(gca,'XLim');
    line(xL,[1.3 1.3],'Color','black'); % line at 1.3 to mark significance level (0.05)
    h=legend('Group','Location','SouthEastOutside');
    clear h;
    title(sprintf('%s %s Corrected Group Local p-values',Fnames{1},params{myparams}));
    
    %save plot
    saveas(gcf,sprintf('%s_%s_Corrected_Group_Local_pvalues.pdf',Fnames{1},params{myparams}),'pdf');
    close();
    
    %% plot ALL betas zoomed in - for a multivariate analysis - NEW way of plotting betas
    % betas are the coefficients that describe how related the covariate is to
    % the parameter
    % This is the new beta plotting from Mihye. Edited by Martin to put all covariates for each diffusion parameter on a single plot
    
    AlphaOM=0.05;  % Alpha level used as significance threshold for plotting Omnibus results
    
    for Dii=1:4 % this may ned to be mii = 1:m
        figure(10)
        pii = 2; % Composite Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',red);
        hold on;
        pii = 3; % Sex Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',purple);
        hold on;
        pii = 4; % GestAgeBirth Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',cyan);
        hold on;
        pii = 5; % DaysSinceBirth Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',blue);
        hold on;
        pii = 6; % MEDUC Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',mustard);
        hold on;
        pii = 7; % Scanner Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',orange);
        hold on;
        pii = 8; % Directions Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',darkred);
        hold on;
        pii = 9; % Twin Status Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',darkgreen);
        hold on
        pii = 10; % ICV Line
        plot(arclength,efitBetas(pii,:,Dii),'Color',turquoise);
        hold on
        
        
        pii = 2; % Composite significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'or','LineWidth',2,'Color', red, 'markerfacecolor', red);
        hold on;
        pii = 3; % Sex significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'og','LineWidth',2,'Color', purple, 'markerfacecolor', purple);
        hold on;
        pii = 4; % GestAgeBirth significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ob','LineWidth',2,'Color', cyan, 'markerfacecolor', cyan);
        hold on;
        pii = 5; % DaysSinceBirth significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'om','LineWidth',2,'Color', blue, 'markerfacecolor', blue);
        hold on;
        pii = 6; % Meduc significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'oy','LineWidth',2,'Color', mustard, 'markerfacecolor', mustard);
        hold on;
        pii = 7; % Scanner significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'oc','LineWidth',2,'Color', orange, 'markerfacecolor', orange);
        hold on;
        pii = 8; % Directions significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ok','LineWidth',2,'Color', darkred, 'markerfacecolor', darkred);
        hold on;
        pii = 9; % TwinStatus significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ob','LineWidth',2,'Color', darkgreen, 'markerfacecolor', darkgreen);
        hold on;
        pii = 10; % ICV significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'oc','LineWidth',2,'Color', turquoise, 'markerfacecolor', turquoise);
        hold off;
        
        
        %     pii = 2; % Composite significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*r','LineWidth',2)
        %       hold on;
        %     pii = 3; % Gender significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*g','LineWidth',2)
        %       hold on;
        %     pii = 4; % GestAgeBirth significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*b','LineWidth',2)
        %       hold on;
        %     pii = 5; % DaysSinceBirth significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*m','LineWidth',2)
        %       hold on;
        %     pii = 6; % Meduc significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*y','LineWidth',2)
        %       hold on;
        %     pii = 7; % Scanner significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*c','LineWidth',2)
        %       hold on;
        %     pii = 8; % Directions significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*k','LineWidth',2)
        %       hold on;
        %     pii = 9; % TwinStatus significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*b','LineWidth',2)
        %       hold on;
        %     pii = 9; % ICV significance fill
        %       ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        %       plot(arclength(ind),efitBetas(pii,ind,Dii),'*c','LineWidth',2)
        %       hold on;
        
        
        xlabel('arclength');
        xlim([min(arclength) max(arclength)]);
        ylabel(Dnames{Dii});
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        title(sprintf('%s %s %s Estimated Coefficient',Fnames{1},Dnames{Dii},covars{mycovars}));
        h=legend(Pnames(2:P),'Location','SouthEastOutside');
        clear h;
        hold off;
        
        % save plot
        figurename=sprintf('%s_%s_%s_All_Betas.pdf',Fnames{1},Dnames{Dii},covars{mycovars});
        saveas(gcf,figurename,'pdf');
        
    end
    
    %% plot SIGNIFICANT BETAS only- ZOOMED IN - for ALL diffusion paramters on one plot -- A plot for each COVARIATE
    
    for pii = 2:P; % each covariate's betas
        figure();
        Dii= 1; % FA
        plot(arclength,efitBetas(pii,:,Dii),'-r');
        hold on;
        Dii = 2; % RD
        plot(arclength,efitBetas(pii,:,Dii),'-g');
        hold on;
        Dii = 3; % AD
        plot(arclength,efitBetas(pii,:,Dii),'-b');
        hold on;
        Dii = 4; % MD
        plot(arclength,efitBetas(pii,:,Dii),'-m');
        hold on;
        
        Dii = 1; % FA significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'or')
        hold on;
        Dii = 2; % RD significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'og')
        hold on;
        Dii = 3; % AD significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ob')
        hold on;
        Dii = 4; % MD significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'om')
        hold on;
        
        Dii = 1; % FA significance filling in circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*r','LineWidth',2)
        hold on;
        Dii = 2; % RD significance filling in circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*g','LineWidth',2)
        hold on;
        Dii = 3; % AD significance filling in circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*b','LineWidth',2)
        hold on;
        Dii = 4; % MD significance filling in circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*m','LineWidth',2)
        hold on;
        
        xlabel('arclength');
        xlim([min(arclength) max(arclength)]);
        ylabel('Beta Values');
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        title(sprintf('%s %s Significant Estimated Coefficients',Fnames{1},Pnames{pii}));
        h=legend(Dnames(1:D),'Location','SouthEastOutside');
        clear h;
        hold off;
        
        % save plot
        figurename=sprintf('%s_%s_SigBetas.pdf',Fnames{1},Pnames{pii});
        saveas(gcf,figurename,'pdf');
        close();
    end
    
    %% plot SIGNIFICANT BETAS only- Y-LIMITED - for ALL diffusion paramters on one plot -- A plot for each COVARIATE
    
    for pii = 2:P; % each covariate's betas
        figure();
        Dii= 1; % FA
        plot(arclength,efitBetas(pii,:,Dii),'-r');
        hold on;
        Dii = 2; % MD
        plot(arclength,efitBetas(pii,:,Dii),'-g');
        hold on;
        Dii = 3; % RD
        plot(arclength,efitBetas(pii,:,Dii),'-b');
        hold on;
        Dii = 4; % AD
        plot(arclength,efitBetas(pii,:,Dii),'-m');
        hold on;
        
        Dii = 1; % FA significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'or')
        hold on;
        Dii = 2; % MD significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'og')
        hold on;
        Dii = 3; % RD significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ob')
        hold on;
        Dii = 4; % AD significance circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'om')
        hold on;
        
        Dii = 1; % FA significance filling in circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*r','LineWidth',2)
        hold on;
        Dii = 2; % MD significance filling in circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*g','LineWidth',2)
        hold on;
        Dii = 3; % RD significance filling in circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*b','LineWidth',2)
        hold on;
        Dii = 4; % AD significance filling in circle
        ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*m','LineWidth',2)
        hold on;
        
        xlabel('arclength');
        xlim([min(arclength) max(arclength)]);
        ylabel('Beta Values');
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        title(sprintf('%s %s Significant Estimated Coefficients',Fnames{1},Pnames{pii}));
        h=legend(Dnames(1:D),'Location','SouthEastOutside');
        clear h;
        hold off;
        
        % save plot
        figurename=sprintf('%s_%s_SigBetas_ylimit.pdf',Fnames{1},Pnames{pii});
        saveas(gcf,figurename,'pdf');
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
    
    
    %% Omnibus Covariate Confidence Bands
    
    [Gvalue] = MVCM_cb_Gval( arclength_allPos, Xdesign, ResYdesign, InvSigmats, mh, GG );
    alpha = 0.05;
    [CBands] = MVCM_CBands( N, alpha, Gvalue, efitBetas, zeros( size( ebiasBetas ) ) ); % new Conf Bands formula
    
    
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% plot omnibus confidence bands
    for Dii=1:D
        for pii=1:P
            figure();
            plot(arclength,efitBetas(pii,:,Dii),'-b','LineWidth', 2); % Parameter (FA,MD,RD,AD)
            hold on;
            plot(arclength,CBands(2*pii-1,:,Dii),'--r','LineWidth', 2); % Lower Conf Band
            hold on;
            plot(arclength,CBands(2*pii,:,Dii),'--r','LineWidth', 2); % Upper Conf Band
            xlabel('arclength');
            ylabel(Dnames{Dii});
            title(sprintf('95 percent confidence band for %s (%s)',Pnames{pii},Dnames{Dii}));
            xlim([min(arclength) max(arclength)]);
            xL = get(gca,'XLim');
            line(xL,[0 0],'Color','black'); % line at zero
            hold off;
            
            % save plot
            figurename=sprintf('%s_%s_%s_confidence_band.pdf',Fnames{1},Dnames{Dii},Pnames{pii});
            saveas(gcf,figurename,'pdf');
            close();
        end
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
    
    
end
% End of Omnibus Hypothesis Test




%% 4. Post-hoc Hypothesis Test --> Which Diffusion Parameter is significant where for each covariate? A univariate test in a multivariate model.
if( postHoc == 1 )
    % for comparing the significance of each diffusion parameter for each covariate
    posthoc_Gpvals = zeros( D, P-1 );
    posthoc_Lpvals = zeros( A, D, P-1 );
    
    for pii = 2:P; % each covariate's betas
        for Dii = 1:D
            Cdesign = zeros( 1, D*P );
            Cdesign( 1+( Dii-1 )*P+( pii-1 ) ) = 1;
            B0vector = zeros( 1, A );
            [Gstat, Lstat] = MVCM_ht_stat( NoSetup, arclength_allPos, Xdesign, efitBetas, eSigEta, Cdesign, B0vector, ebiasBetas );
            
            % Generate random samples and calculate the corresponding statistics and pvalues
            GG = numPerms;
            posthoc_Gpvals( Dii, pii-1 ) =  MVCM_bstrp_pvalue3( NoSetup, arclength_allPos, Xdesign, Ydesign, efitBetas1, InvSigmats, mh, Cdesign, B0vector, Gstat, GG );
            posthoc_Lpvals( :, Dii, pii-1 ) = 1-chi2cdf( Lstat, 1 );
        end
    end
    
    % Global P values for posthoc test
    
    posthoc_Gpvals % for FA, RD, AD, MD for each covariate
    
    % Save Post-hoc test Global p-values for each diffusion parameter
    
    csvwrite( sprintf( '%s/%s_posthoc_Global_pvalues.csv', folder, Fnames{1} ), posthoc_Gpvals ); %save csv file
    
    %% correct posthoc test local p-values with FDR
    % this corrects the posthoc local p-values for multiple comparisons
    
    posthoc_Lpvals_FDR = zeros( size( posthoc_Lpvals ) );
    for Dii = 1:D
        for pii = 1:( P-1 )
            posthoc_Lpvals_FDR( :, Dii, pii ) = mafdr( posthoc_Lpvals( :, Dii, pii ), 'BHFDR', true );
        end
    end
    
    % save FDR Local P-Values csv file
    for Dii = 1:D
        csvwrite( sprintf( '%s/%s_%s_posthoc_Local_pvalues.csv', folder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals( :, Dii, : ) );
        csvwrite( sprintf( '%s/%s_%s_posthoc_FDR_Local_pvalues.csv', folder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals_FDR( :, Dii, : ) );
    end
    
    
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% Plot CORRECTED Post-hoc test Local P-values for Each Difffusion Parameter
    
    for pii = 2:P; % each covariate's betas
        figure();
        plot(arclength,-log10(posthoc_Lpvals_FDR(:,1,pii-1)),'-r.','LineWidth', 2,'MarkerSize',15); % FA
        hold on;
        plot(arclength,-log10(posthoc_Lpvals_FDR(:,2,pii-1)),'-g.','LineWidth', 2,'MarkerSize',15); % RD
        hold on;
        plot(arclength,-log10(posthoc_Lpvals_FDR(:,3,pii-1)),'-b.','LineWidth', 2,'MarkerSize',15); % AD
        hold on;
        plot(arclength,-log10(posthoc_Lpvals_FDR(:,4,pii-1)),'-m.','LineWidth', 2,'MarkerSize',15); % MD
        hold off;
        xlabel('arclength');
        xlim([min(arclength) max(arclength)]);
        ylabel('-log10(p)');
        xL = get(gca,'XLim');
        line(xL,[1.3 1.3],'Color','black'); % line at 1.3 to mark significance level
        legend([Dnames(1:D)],'Location','SouthEastOutside');
        title(sprintf('%s %s Posthoc Corrected Local p-values',Fnames{1},Pnames{pii}));
        
        % save local p-value plot
        saveas(gcf,sprintf('%s_%s_posthoc_corrected_Local_pvalues.pdf',Fnames{1},Pnames{pii}),'pdf');
        close();
    end
    
    %% plot CORRECTED Post-Hoc Test SIGNIFICANT BETAS only- ZOOOMED IN - for ALL diffusion paramters on one plot -- A plot for each COVARIATE
    
    AlphaPH=0.05;
    
    for pii = 2:P; % each covariate's betas
        figure()
        Dii= 1; % FA
        plot(arclength,efitBetas(pii,:,Dii),'-r');
        hold on;
        Dii = 2; % RD
        plot(arclength,efitBetas(pii,:,Dii),'-g');
        hold on;
        Dii = 3; % AD
        plot(arclength,efitBetas(pii,:,Dii),'-b');
        hold on;
        Dii = 4; % MD
        plot(arclength,efitBetas(pii,:,Dii),'-m');
        hold on;
        
        Dii = 1; % FA significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'or')
        hold on;
        Dii = 2; % RD significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'og')
        hold on;
        Dii = 3; % AD significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ob')
        hold on;
        Dii = 4; % MD significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'om')
        hold on;
        
        Dii = 1; % FA significance filling in circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*r','LineWidth',2)
        hold on;
        Dii = 2; % RD significance filling in circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*g','LineWidth',2)
        hold on;
        Dii = 3; % AD significance filling in circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*b','LineWidth',2)
        hold on;
        Dii = 4; % MD significance filling in circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*m','LineWidth',2)
        hold on;
        
        xlabel('arclength');
        xlim([min(arclength) max(arclength)]);
        ylabel('Beta Values');
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        title(sprintf('%s %s Post-Hoc Significant Estimated Coefficients',Fnames{1},Pnames{pii}));
        h=legend(Dnames(1:D),'Location','SouthEastOutside');
        clear h;
        hold off;
        
        % save plot
        figurename=sprintf('%s_%s_posthoc_corrected_SigBetas.pdf',Fnames{1},Pnames{pii});
        saveas(gcf,figurename,'pdf');
        close()
    end
    
    %% plot CORRECTED Post-Hoc Test SIGNIFICANT BETAS only- Y AXIS LIMITS - for ALL diffusion paramters on one plot -- A plot for each COVARIATE
    
    for pii = 2:P; % each covariate's betas
        figure(16)
        Dii= 1; % FA
        plot(arclength,efitBetas(pii,:,Dii),'-r');
        hold on;
        Dii = 2; % RD
        plot(arclength,efitBetas(pii,:,Dii),'-g');
        hold on;
        Dii = 3; % AD
        plot(arclength,efitBetas(pii,:,Dii),'-b');
        hold on;
        Dii = 4; % MD
        plot(arclength,efitBetas(pii,:,Dii),'-m');
        hold on;
        
        Dii = 1; % FA significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'or')
        hold on;
        Dii = 2; % RD significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'og')
        hold on;
        Dii = 3; % AD significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ob')
        hold on;
        Dii = 4; % MD significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'om')
        hold on;
        
        Dii = 1; % FA significance filling in circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*r','LineWidth',2)
        hold on;
        Dii = 2; % RD significance filling in circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*g','LineWidth',2)
        hold on;
        Dii = 3; % AD significance filling in circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*b','LineWidth',2)
        hold on;
        Dii = 4; % MD significance filling in circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'*m','LineWidth',2)
        hold on;
        
        xlabel('arclength');
        xlim([min(arclength) max(arclength)]);
        ylabel('Beta Values');
        ylim([-0.03 0.02]); % Change this limit to SouthEastOutside fit your data for visualization
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        title(sprintf('%s %s Post-Hoc Significant Estimated Coefficients',Fnames{1},Pnames{pii}));
        h=legend(Dnames(1:D),'Location','SouthEastOutside');
        clear h;
        hold off;
        
        % save plot
        figurename=sprintf('%s_%s_posthoc_corrected_SigBetas_ylimit.pdf',Fnames{1},Pnames{pii});
        saveas(gcf,figurename,'pdf');
        close()
    end
    
    %% plot ALL betas zoomed in - for a multivariate analysis - NEW way of plotting betas --POSTHOC CORRECTED
    % betas are the coefficients that describe how related the covariate is to
    % the parameter
    % This is the new beta plotting from Mihye. Edited by Martin (then re-edited by Jessica) to put all covariates for each diffusion parameter on a single plot
    
    AlphaPH=0.05;  % Alpha level used as significance threshold for plotting Omnibus results
    
    for Dii=1:4
        figure(10)
        pii = 2; % Composite Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', red);
        hold on;
        pii = 3; % Sex Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', purple);
        hold on;
        pii = 4; % GestAgeBirth Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', cyan);
        hold on;
        pii = 5; % DaysSinceBirth Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', blue);
        hold on;
        pii = 6; % MEDUC Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', mustard);
        hold on;
        pii = 7; % Scanner Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', orange);
        hold on;
        pii = 8; % Directions Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', darkred);
        hold on;
        pii = 9; % Twin Status Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', darkgreen);
        hold on
        pii = 10; % ICV Line
        plot(arclength,efitBetas(pii,:,Dii),'Color', turquoise);
        hold on
        
        
        pii = 2; % Composite significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'or','LineWidth',2, 'Color', red, 'markerfacecolor', red);
        hold on;
        pii = 3; % Sex significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'og','LineWidth',2, 'Color', purple, 'markerfacecolor', purple);
        hold on;
        pii = 4; % GestAgeBirth significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ob','LineWidth',2, 'Color', cyan, 'markerfacecolor', cyan);
        hold on;
        pii = 5; % DaysSinceBirth significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'om','LineWidth',2, 'Color', blue, 'markerfacecolor', blue);
        hold on;
        pii = 6; % Meduc significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'oy','LineWidth',2, 'Color', mustard, 'markerfacecolor', mustard);
        hold on;
        pii = 7; % Scanner significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'oc','LineWidth',2, 'Color', orange, 'markerfacecolor', orange);
        hold on;
        pii = 8; % Directions significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ok','LineWidth',2, 'Color', darkred, 'markerfacecolor', darkred);
        hold on;
        pii = 9; % TwinStatus significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'ob','LineWidth',2, 'Color', darkgreen, 'markerfacecolor', darkgreen);
        hold on;
        pii = 10; % ICV significance circle
        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        plot(arclength(ind),efitBetas(pii,ind,Dii),'oc','LineWidth',2, 'Color', turquoise, 'markerfacecolor', turquoise);
        hold off;
        
        
        %      pii = 2; % Composite significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*r','LineWidth',2)
        %        hold on;
        %      pii = 3; % Gender significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*g','LineWidth',2)
        %        hold on;
        %      pii = 4; % GestAgeBirth significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*b','LineWidth',2)
        %        hold on;
        %      pii = 5; % DaysSinceBirth significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*m','LineWidth',2)
        %        hold on;
        %      pii = 6; % Meduc significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*y','LineWidth',2)
        %        hold on;
        %       pii = 7; % Scanner significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*c','LineWidth',2)
        %        hold on;
        %      pii = 8; % Directions significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*k','LineWidth',2)
        %        hold on;
        %      pii = 9; % TwinStatus significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*b','LineWidth',2)
        %        hold on;
        %      pii = 10; % TwinStatus significance fill
        %        ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
        %        plot(arclength(ind),efitBetas(pii,ind,Dii),'*c','LineWidth',2)
        %        hold on;
        
        
        xlabel('arclength');
        xlim([min(arclength) max(arclength)]);
        ylabel(Dnames{Dii});
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        title(sprintf('%s %s %s Post-Hoc Significant Estimated Coefficient',Fnames{1},Dnames{Dii},covars{mycovars}));
        h=legend(Pnames(2:P),'Location','SouthEastOutside');
        clear h;
        hold off;
        
        % save plot
        figurename=sprintf('%s_%s_%s_posthoc_corrected_All_Betas.pdf',Fnames{1},Dnames{Dii},covars{mycovars});
        saveas(gcf,figurename,'pdf');
        
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
      
    
    
end
% End of Post-hoc Hypothesis Test
