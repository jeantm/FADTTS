%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%       FADTTS Analysis       %%%%%%%%%%%%
%%%%%%%%%%%% FADTTS Executable $version$ %%%%%%%%%%%%
%%%%%%%%%%%%    $date$ at $time$   %%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all
clear all
clc

%%% Path for FADTTS functions in matlab %%%
$addMVCMPath$


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%% Inputs %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%% savingFolder %%%
[ savingFolder, savingName, savingExt ] = fileparts( mfilename( 'fullpath' ) );

%%% loadingFolder %%%
loadingFolder = strcat( savingFolder, '/..' )

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
numPerms = nbrPermutations;


%% COVARIATES
% this file has all covariates organized in columns (without headings):
$matlabCOMPInputFile$
designdata = [ ones( size( data2, 1 ), 1 ) data2 ]; % intercept + covariates

Pnames = cell( nbrCovariates, 1 );
$covariates$

%Covariates Tested for file names
covars=cell(1,1);
covars{1}='ALLcovars';

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
nbrSubjects = NoSetup( 1 ); % No of subjects
nbrArclengths = NoSetup( 2 ); % No of arclengths
nbrCovariates = NoSetup( 3 ); % No of covariates (including intercept)
nbrDiffusionProperties = NoSetup( 4 ); % No of diffusion properties = 1




%% 2. fit a model using local polynomial kernel smoothing
% calculating Betas
[ mh ] = MVCM_lpks_wob( NoSetup, arclength_allPos, Xdesign, Ydesign );
[ efitBetas, efitBetas1, InvSigmats, efitYdesign ] = MVCM_lpks_wb1( NoSetup, arclength_allPos, Xdesign, Ydesign, mh );

% save Beta txt file
for i = 1:nbrDiffusionProperties
    savefile=sprintf('%s/%s_%s_betas.csv', savingFolder, Fnames{1}, Dnames{i});
    temp=efitBetas(:,:,i);
    csvwrite(savefile,temp);
    clear temp;
end


%% smoothing individual function
ResYdesign = Ydesign - efitYdesign;
[ ResEtas, efitEtas, eSigEta ] = MVCM_sif( arclength_allPos, ResYdesign );
[ mSigEtaEig, mSigEta ] = MVCM_eigen( efitEtas );




%% 3. Omnibus Hypothesis Test
if( omnibus == 1 )
    Gstats = zeros( 1, nbrCovariates-1 );
    Lstats = zeros( nbrArclengths, nbrCovariates-1 );
    Gpvals = zeros( 1, nbrCovariates-1 );
    
    [ ebiasBetas ] = MVCM_bias( NoSetup, arclength_allPos, Xdesign, Ydesign, InvSigmats, mh );
    
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
    
    Lpvals = 1-chi2cdf( Lstats, nbrDiffusionProperties );
    

    %% Global p-value
    Gpvals
    csvwrite( sprintf( '%s/%s_%s_Global_pvalues.csv', savingFolder, Fnames{1}, params{1} ), Gpvals ); %save csv file
    

    %% correct local p-values with FDR
    % this corrects the local p-values for multiple comparisons
    Lpvals_FDR = zeros( size( Lpvals ) );
    for i = 1:( nbrCovariates-1 )
        Lpvals_FDR( :, i ) = mafdr( Lpvals( :, i ), 'BHFDR', true );
    end
    
    % save FDR Local nbrCovariates-Values csv file
    csvwrite( sprintf( '%s/%s_%s_FDR_Local_pvalues.csv', savingFolder, Fnames{1}, params{1} ), Lpvals_FDR );
    

    %% Omnibus Covariate Confidence Bands
    [Gvalue] = MVCM_cb_Gval( arclength_allPos, Xdesign, ResYdesign, InvSigmats, mh, GG );
    alpha = 0.05;
    [CBands] = MVCM_CBands( nbrSubjects, alpha, Gvalue, efitBetas, zeros( size( ebiasBetas ) ) ); % new Conf Bands formula
end
% End of Omnibus Hypothesis Test




%% 4. Post-hoc Hypothesis Test --> Which Diffusion Parameter is significant where for each covariate? nbrArclengths univariate test in a multivariate model.
if( postHoc == 1 )
    % for comparing the significance of each diffusion parameter for each covariate
    posthoc_Gpvals = zeros( nbrDiffusionProperties, nbrCovariates-1 );
    posthoc_Lpvals = zeros( nbrArclengths, nbrDiffusionProperties, nbrCovariates-1 );
    
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
    csvwrite( sprintf( '%s/%s_posthoc_Global_pvalues.csv', savingFolder, Fnames{1} ), posthoc_Gpvals ); %save csv file
    

    %% correct posthoc test local p-values with FDR
    % this corrects the posthoc local p-values for multiple comparisons
    posthoc_Lpvals_FDR = zeros( size( posthoc_Lpvals ) );
    for Dii = 1:nbrDiffusionProperties
        for pii = 1:( nbrCovariates-1 )
            posthoc_Lpvals_FDR( :, Dii, pii ) = mafdr( posthoc_Lpvals( :, Dii, pii ), 'BHFDR', true );
        end
    end
    
    % save FDR Local nbrCovariates-Values csv file
    for Dii = 1:nbrDiffusionProperties
        csvwrite( sprintf( '%s/%s_%s_posthoc_Local_pvalues.csv', savingFolder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals( :, Dii, : ) );
        csvwrite( sprintf( '%s/%s_%s_posthoc_FDR_Local_pvalues.csv', savingFolder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals_FDR( :, Dii, : ) );
    end
end
% End of Post-hoc Hypothesis Test
