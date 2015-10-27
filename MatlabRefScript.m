%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%       FADTTS Analysis       %%%%%%%%%%%%
%%%%%%%%%%%% FADTTS Executable $version$ %%%%%%%%%%%%
%%%%%%%%%%%%    $date$ at $time$   %%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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

% Fiber Names
Fnames = cell( 1, 1 );
Fnames{ 1 } = fiberName;

% Diffusion Properties
Dnames = cell( 2, 1 );
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

diffusionFiles = cell( 2, 1 );
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




%% 2. fit a model using local polynomial kernel smoothing
% calculating Betas

[ mh ] = MVCM_lpks_wob( NoSetup, arclength_allPos, Xdesign, Ydesign );
[ efitBetas, efitBetas1, InvSigmats, efitYdesign ] = MVCM_lpks_wb1( NoSetup, arclength_allPos, Xdesign, Ydesign, mh );

%% smoothing individual function  

ResYdesign = Ydesign - efitYdesign;
[ ResEtas, efitEtas, eSigEta ] = MVCM_sif( arclength_allPos, ResYdesign );
[ mSigEtaEig, mSigEta ] = MVCM_eigen( efitEtas );




%% 3. Omnibus Hypothesis Test
if( omnibus == 1 )
    Gstats = zeros( 1, P-1 );
    Lstats = zeros( A, P-1 );
    Gpvals = zeros( 1, P-1 );

    [ ebiasBetas ] = MVCM_bias( NoSetup, arclength_allPos, Xdesign, Ydesign, InvSigmats, mh );

    % save Beta txt file
%    for i = 1:size( Dnames )
%        savefile=sprintf('%s/%s_%s_%s_betas.csv', folder, Fnames{1}, Dnames{i} ,params{1});
%        temp=efitBetas(:,:,i);
%        csvwrite(savefile,temp');
%        clear temp;
%    end

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

    %% Global p-value

    Gpvals

    csvwrite( sprintf( '%s/%s_%s_Global_pvalues.csv', folder, Fnames{1}, params{1} ), Gpvals ); %save csv file

    %% correct local p-values with FDR
    % this corrects the local p-values for multiple comparisons

    Lpvals_FDR = zeros( size( Lpvals ) );
    for i = 1:( P-1 )
%         Lpvals_FDR( :, i ) = myFDR( Lpvals( :, i ) );
        Lpvals_FDR( :, i ) = mafdr( Lpvals( :, i ), 'BHFDR', true );
    end

    % save FDR Local P-Values csv file
    csvwrite( sprintf( '%s/%s_%s_FDR_Local_pvalues.csv', folder, Fnames{1}, params{1} ), Lpvals_FDR );

    %% Omnibus Covariate Confidence Bands

    [Gvalue] = MVCM_cb_Gval( arclength_allPos, Xdesign, ResYdesign, InvSigmats, mh, GG );
    alpha = 0.05;
    [CBands] = MVCM_CBands( N, alpha, Gvalue, efitBetas, zeros( size( ebiasBetas ) ) ); % new Conf Bands formula
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
%             posthoc_Lpvals_FDR( :, Dii, pii ) = myFDR( posthoc_Lpvals( :, Dii, pii ) );
            posthoc_Lpvals_FDR( :, Dii, pii ) = mafdr( posthoc_Lpvals( :, Dii, pii ), 'BHFDR', true );
        end
    end

    % save FDR Local P-Values csv file
    for Dii = 1:D
        csvwrite( sprintf( '%s/%s_%s_posthoc_Local_pvalues.csv', folder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals( :, Dii, : ) );
        csvwrite( sprintf( '%s/%s_%s_posthoc_FDR_Local_pvalues.csv', folder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals_FDR( :, Dii, : ) );
    end
end
% End of Post-hoc Hypothesis Test
