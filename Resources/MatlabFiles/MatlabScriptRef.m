%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%    FADTTS Processing   %%%%%%%%%%%%
%%%%%%%%%%%     FADTTSter $version$     %%%%%%%%%%%%
%%%%%%%%%%% $date$ at $time$ %%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all
clear all
clc

disp('Running matlab script...')

%%% Path for FADTTS functions in matlab %%%
$addMVCMPath$


disp('1. Set/Load')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%% Inputs %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Setting inputs...')
%%% loadingFolder %%%
[ loadingFolder, loadingName, loadingExt ] = fileparts( mfilename( 'fullpath' ) );

%%% savingFolder %%%
savingFolder = strcat( loadingFolder, '/MatlabOutputs' );

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


disp('Loading covariate file...')
%% COVARIATES
% this file has all covariates organized in columns (without headings):
$matlabCOMPInputFile$
designdata = [ ones( size( data2, 1 ), 1 ) data2 ]; % intercept + covariates

Pnames = cell( nbrCovariates, 1 );
$covariates$

disp('Loading fiber data file...')
%% LOAD FIBER DATA
% diffusionFiles should be a (arclength)X(subject) matrix.
% the first column has arc-lengths
$diffusionFiles$

disp('Processing arclength...')
%% ARCLENGTH
% reading (x,y,z) coordinates

% Get arclength from input file
arclength = dataFiber1All( :, 1 ); % take first column => arclength from dtiCC_statCLP fiber file

% Creating (x,y,z) coordinates
CC_data = [ arclength zeros( size( arclength, 1 ), 1 ) zeros( size( arclength, 1 ), 1 ) ];

nofeatures = size( diffusionFiles, 1 );
[ NoSetup, arclength_allPos, Xdesign, Ydesign ] = MVCM_read( CC_data, designdata, diffusionFiles, nofeatures );
nbrSubjects = NoSetup( 1 );	% No of subjects
nbrArclengths = NoSetup( 2 ); % No of arclengths
nbrCovariates = NoSetup( 3 ); % No of covariates (including intercept)
nbrDiffusionProperties = NoSetup( 4 );	% No of diffusion properties = 1

//disp('Calculating and saving raw statistics...');
//for pii=1:nbrCovariates
//    [Mavg]= mean(Ydesign(designdata(:,pii)==0,:,:)); % group 0 average for each diffusion paramter
//    [Mstddev]= std(Ydesign(designdata(:,pii)==0,:,:)); % group 0 standard deviation for each diffusion paramter
//    [Favg]= mean(Ydesign(designdata(:,pii)==1,:,:)); % group 1 average for each diffusion paramter
//    [Fstddev]= std(Ydesign(designdata(:,pii)==1,:,:)); % group 1 standard deviation for each diffusion paramter
//    for Dii=1:nbrDiffusionProperties
//        csvwrite( sprintf('%s/%s_RawStats_group0_Average_%s_%s.csv',savingFolder,Fnames{1},Dnames{Dii},Pnames{pii}), Mavg(:,:,Dii)+Mstddev(:,:,Dii) );
//        csvwrite( sprintf('%s/%s_RawStats_group0_Standard_%s_%s.csv',savingFolder,Fnames{1},Dnames{Dii},Pnames{pii}), Mavg(:,:,Dii)-Mstddev(:,:,Dii) );
//        csvwrite( sprintf('%s/%s_RawStats_group1_Average_%s_%s.csv',savingFolder,Fnames{1},Dnames{Dii},Pnames{pii}), Favg(:,:,Dii)+Fstddev(:,:,Dii) );
//        csvwrite( sprintf('%s/%s_RawStats_group1_Standard_%s_%s.csv',savingFolder,Fnames{1},Dnames{Dii},Pnames{pii}), Favg(:,:,Dii)-Fstddev(:,:,Dii) );
//    end
//end


disp('2. Betas')
%% 2. fit a model using local polynomial kernel smoothing
disp('Calculating betas...')
% calculating Betas
[ mh ] = MVCM_lpks_wob( NoSetup, arclength_allPos, Xdesign, Ydesign );
[ efitBetas, efitBetas1, InvSigmats, efitYdesign ] = MVCM_lpks_wb1( NoSetup, arclength_allPos, Xdesign, Ydesign, mh );

disp('Saving betas...')
% save Beta csv file
for i = 1:nbrDiffusionProperties
    csvwrite(sprintf('%s/%s_%s_Betas.csv', savingFolder, Fnames{1}, Dnames{i}),efitBetas(:,:,i));
end


disp('Smoothing individual function...')
%% smoothing individual function
ResYdesign = Ydesign - efitYdesign;
[ ResEtas, efitEtas, eSigEta ] = MVCM_sif( arclength_allPos, ResYdesign );
[ mSigEtaEig, mSigEta ] = MVCM_eigen( efitEtas );




disp('3. Omnibus')
%% 3. Omnibus Hypothesis Test
if( omnibus == 1 )
    Gstats = zeros( 1, nbrCovariates-1 );
    Lstats = zeros( nbrArclengths, nbrCovariates-1 );
    Gpvals = zeros( 1, nbrCovariates-1 );
    
    disp('Calculating bias...')
    [ ebiasBetas ] = MVCM_bias( NoSetup, arclength_allPos, Xdesign, Ydesign, InvSigmats, mh );
    
    disp('Calculating individual and global statistics...')
    for pp=2:nbrCovariates
        %individual and global statistics calculation
        cdesign=zeros( 1, nbrCovariates );
        cdesign( pp ) = 1;
        Cdesign = kron( eye( nbrDiffusionProperties ), cdesign );
        B0vector = zeros( nbrDiffusionProperties, nbrArclengths );
        [Gstat, Lstat] = MVCM_ht_stat( NoSetup, arclength_allPos, Xdesign, efitBetas, eSigEta, Cdesign, B0vector, ebiasBetas );
        Gstats( 1, pp-1 ) = Gstat;
        Lstats( :, pp-1 ) = Lstat;
        
        % Generate random samples and calculate the corresponding statistics and pvalues
        GG = numPerms;
        [Gpval] = MVCM_bstrp_pvalue3( NoSetup, arclength_allPos, Xdesign, Ydesign, efitBetas1, InvSigmats, mh, Cdesign, B0vector, Gstat, GG );
        Gpvals( 1, pp-1 ) = Gpval;
    end
    

    %% Local p-value
    Lpvals = 1-chi2cdf( Lstats, nbrDiffusionProperties );
    disp('Saving local p-values...')
    %save Local nbrCovariates-Values csv file
    csvwrite( sprintf('%s/%s_%s_Omnibus_Local_pvalues.csv', savingFolder, Fnames{1}, params{1} ), Lpvals );  % column for each covariate; local p-values are computed at each arclength

    %% Global p-value
    Gpvals
    disp('Saving global p-values...')
    csvwrite( sprintf( '%s/%s_%s_Omnibus_Global_pvalues.csv', savingFolder, Fnames{1}, params{1} ), Gpvals ); % save csv file
    
    disp('Correcting local p-values...')
    %% correct local p-values with FDR
    % this corrects the local p-values for multiple comparisons
    Lpvals_FDR = zeros( size( Lpvals ) );
    for i = 1:( nbrCovariates-1 )
        Lpvals_FDR( :, i ) = myFDR( Lpvals( :, i ));
    end
    
    % save FDR Local nbrCovariates-Values csv file
    disp('Saving FDR local p-values...')
    csvwrite( sprintf( '%s/%s_%s_Omnibus_FDR_Local_pvalues.csv', savingFolder, Fnames{1}, params{1} ), Lpvals_FDR );
    

    disp('Calculating omnibus covariate confidence bands...')
    %% Omnibus Covariate Confidence Bands
    [Gvalue] = MVCM_cb_Gval( arclength_allPos, Xdesign, ResYdesign, InvSigmats, mh, GG );
    alpha = 0.05;
    [CBands] = MVCM_CBands( nbrSubjects, alpha, Gvalue, efitBetas, zeros( size( ebiasBetas ) ) ); % new Conf Bands formula
    disp('Saving omnibus covariate confidence bands...')
    csvwrite( sprintf( '%s/%s_Omnibus_ConfidenceBand.csv', savingFolder, Fnames{1}, Dnames{Dii}, Pnames{pii}), CBands );
end
% End of Omnibus Hypothesis Test



disp('4. Post-hoc')
%% 4. Post-hoc Hypothesis Test --> Which Diffusion Parameter is significant where for each covariate? nbrArclengths univariate test in a multivariate model.
if( postHoc == 1 )
    disp('Comparing the significance of each diffusion parameter for each covariate...')
    % for comparing the significance of each diffusion parameter for each covariate
    posthoc_Gpvals = zeros( nbrDiffusionProperties, nbrCovariates-1 );
    posthoc_Lpvals = zeros( nbrArclengths, nbrDiffusionProperties, nbrCovariates-1 );
    
    disp('Calculating individual and global statistics...')
    for pii = 2:nbrCovariates; % each covariate's betas
        for Dii = 1:nbrDiffusionProperties
            Cdesign = zeros( 1, nbrDiffusionProperties*nbrCovariates );
            Cdesign( 1+( Dii-1 )*nbrCovariates+( pii-1 ) ) = 1;
            B0vector = zeros( 1, nbrArclengths );
            [Gstat, Lstat] = MVCM_ht_stat( NoSetup, arclength_allPos, Xdesign, efitBetas, eSigEta, Cdesign, B0vector, ebiasBetas );
            
            % Generate random samples and calculate the corresponding statistics and pvalues
            GG = numPerms;
            posthoc_Gpvals( Dii, pii-1 ) =  MVCM_bstrp_pvalue3( NoSetup, arclength_allPos, Xdesign, Ydesign, efitBetas1, InvSigmats, mh, Cdesign, B0vector, Gstat, GG );
            posthoc_Lpvals( :, Dii, pii-1 ) = 1-chi2cdf( Lstat, 1 );
        end
    end
    
    % Global nbrCovariates values for posthoc test
    posthoc_Gpvals % for FA, RD, AD, MD for each covariate
    
    % Save Post-hoc test Global p-values for each diffusion parameter
    disp('Saving post-hoc test global p-values for each diffusion parameter...')
    csvwrite( sprintf( '%s/%s_PostHoc_Global_pvalues.csv', savingFolder, Fnames{1} ), posthoc_Gpvals ); %save csv file
    

    disp('Correcting post-hoc local p-values...')
    %% correct posthoc test local p-values with FDR
    % this corrects the posthoc local p-values for multiple comparisons
    posthoc_Lpvals_FDR = zeros( size( posthoc_Lpvals ) );
    for Dii = 1:nbrDiffusionProperties
        for pii = 1:( nbrCovariates-1 )
            posthoc_Lpvals_FDR( :, Dii, pii ) = myFDR( posthoc_Lpvals( :, Dii, pii ) );
        end
    end
    
    % save FDR Local nbrCovariates-Values csv file
    disp('Saving FDR local p-values...')
    for Dii = 1:nbrDiffusionProperties
        csvwrite( sprintf( '%s/%s_%s_PostHoc_Local_pvalues.csv', savingFolder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals( :, Dii, : ) );
        csvwrite( sprintf( '%s/%s_%s_PostHoc_FDR_Local_pvalues.csv', savingFolder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals_FDR( :, Dii, : ) );
    end
end
% End of Post-hoc Hypothesis Test
disp('End of script.')
