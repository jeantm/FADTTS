%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%    FADTTS Processing   %%%%%%%%%%%%
%%%%%%%%%%%     FADTTSter $version$     %%%%%%%%%%%%
%%%%%%%%%%% $date$ at $time$ %%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all
clear all
clc

disp('Running matlab script with plotting...')

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
$inputMatlabSubMatrixInputFile$
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
%Y_LIMITE
% Change this limit to SouthEastOutside fit your data for visualization
ylimMin = -0.02;
ylimMax = 0.015;

%COLORS
marron = [0.5 0 0];
red = [1 0 0];
orange = [1 0.647 0];
yellow = [1 0.843 0];
green = [0 0.392 0];
lime = [0 1 0];
darkCyan = [0 0.545 0.545];
cyan = [0 1 1];
blue = [0 0 1];
violet = [0.541 0.169 0.887];
magenta = [1 0 1];

color = cell(11,1);
color{1}=red;
color{2}=cyan;
color{3}=green;
color{4}=violet;
color{5}=orange;
color{6}=yellow;
color{7}=magenta;
color{8}=darkCyan;
color{9}=blue;
color{10}=marron;
color{11}=lime;

% Fiber Names
Fnames = cell( 1, 1 );
Fnames{ 1 } = fiberName;

% Diffusion Properties
$diffusionProperties$
% Diffusion Properties Tested for file names
params = cell( 1, 1 );
$allProperties$

numPerms = nbrPermutations; % Use 100 when testing scripts. Use 1000 when running scripts for real.


disp('Loading covariate file...')
%% COVARIATES
$matlabSubMatrixInputFile$
designdata = [ ones( size( data2, 1 ), 1 ) data2 ]; % intercept + covariates

Pnames = cell( nbrCovariates, 1 );
$covariates$

disp('Loading fiber data file...')
%% LOAD FIBER DATA
$diffusionFiles$

disp('Processing arclength...')
%% ARCLENGTH
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




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% PLOT CC_Genu PARAMETER - raw data.
for pii=2:nbrCovariates
    if (designdata(1,pii) == 0 || designdata(1,pii) == 1)
        figure()
        for Dii=1:nbrDiffusionProperties
            hold on
            for nii=1:nbrSubjects
                if (designdata(nii,pii) == 0)
                    h(1)=plot(arclength,Ydesign(nii,:,Dii),'-k','LineWidth', 1, 'LineSmoothing', 'ON');
                else
                    h(2)=plot(arclength,Ydesign(nii,:,Dii),'-','Color',color{Dii},'LineWidth', 1, 'LineSmoothing', 'ON');
                end
            end
            hold off
            
            xlabel('Arc Length','fontweight','bold');
            ylabel(Dnames{Dii},'fontweight','bold');
            xlim([min(arclength) max(arclength)]);
            legend([h(1) h(2)],sprintf('%s=0',Pnames{pii}),sprintf('%s=1',Pnames{pii}),'Location','SouthEastOutside');
            title(sprintf('%s %s',Fnames{1},Dnames{Dii}),'fontweight','bold');
            clear h;
            
            save plot
            saveas(gcf,sprintf('%s/%s_%s_%s.pdf',savingFolder,Fnames{1},Dnames{Dii},Pnames{pii}),'pdf');
        end
        close()
    end
end


%% Plot Raw Data Average and Standard Deviation
for pii=2:nbrCovariates
    if (designdata(1,pii) == 0 || designdata(1,pii) == 1)
        [Mavg]= mean(Ydesign(designdata(:,pii)==0,:,:)); % TD average for each diffusion paramter
        [Mstddev]= std(Ydesign(designdata(:,pii)==0,:,:)); % TD standard deviation for each diffusion paramter
        [Favg]= mean(Ydesign(designdata(:,pii)==1,:,:)); % SE average for each diffusion paramter
        [Fstddev]= std(Ydesign(designdata(:,pii)==1,:,:)); % SE standard deviation for each diffusion paramter
        for Dii=1:nbrDiffusionProperties
            figure(Dii)
            hold on
            h(1)=plot(arclength, Mavg(:,:,Dii),'-k','LineWidth', 1.25, 'LineSmoothing', 'ON');
            plot(arclength, Mavg(:,:,Dii)+Mstddev(:,:,Dii),'--k','LineWidth', 1.25, 'LineSmoothing', 'ON');
            plot(arclength, Mavg(:,:,Dii)-Mstddev(:,:,Dii),'--k','LineWidth', 1.25, 'LineSmoothing', 'ON');
            
            h(2)=plot(arclength, Favg(:,:,Dii),'-','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            plot(arclength, Favg(:,:,Dii)+Fstddev(:,:,Dii),'--','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            plot(arclength, Favg(:,:,Dii)-Fstddev(:,:,Dii),'--','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            hold off
            
            xlabel('Arc Length','fontweight','bold');
            ylabel(Dnames{Dii},'fontweight','bold');
            xlim([min(arclength) max(arclength)]);
            legend([h(1) h(2)],sprintf('%s=0',Pnames{pii}),sprintf('%s=1',Pnames{pii}),'Location','SouthEastOutside');
            title(sprintf('%s %s Average and Standard Deviation',Fnames{1},Dnames{Dii}),'fontweight','bold');
            clear h;
            
            save plot
            saveas(gcf,sprintf('%s/%s_%s_Avg_StdDev_%s.pdf',savingFolder,Fnames{1},Dnames{Dii},Pnames{pii}),'pdf');
            close(Dii)
        end
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



disp('2. Betas')
%% 2. fit a model using local polynomial kernel smoothing
disp('Calculating betas...')
[ mh ] = MVCM_lpks_wob( NoSetup, arclength_allPos, Xdesign, Ydesign );
[ efitBetas, efitBetas1, InvSigmats, efitYdesign ] = MVCM_lpks_wb1( NoSetup, arclength_allPos, Xdesign, Ydesign, mh );




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% plot betas - old way of plotting betas - zoomed out with intercept
% betas are the coefficients that describe how related the covariate is to
% the parameter
for Dii=1:nbrDiffusionProperties
    figure()
    hold on
    h(1)=plot(arclength,efitBetas(1,:,Dii),'-k','LineWidth', 1.25, 'LineSmoothing', 'ON');
    for pii=2:nbrCovariates
        h(pii)=plot(arclength,efitBetas(pii,:,Dii),'Color',color{pii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
    end
    hold off
    
    xlabel('Arc Length','fontweight','bold');
    ylabel(Dnames{Dii},'fontweight','bold');
    xlim([min(arclength) max(arclength)]);
    legend([h(:)],Pnames(1:nbrCovariates),'Location','SouthEastOutside');
    title(sprintf('%s %s Beta Values',Fnames{1},Dnames{Dii}),'fontweight','bold');
    clear h;
    
    % save plot
    saveas(gcf,sprintf('%s/%s_%s_Betas.pdf',savingFolder,Fnames{1},Dnames{Dii}),'pdf');
    
    disp('Saving betas...')
    csvwrite(sprintf('%s/%s_%s_Betas.csv', savingFolder, Fnames{1}, Dnames{Dii}),efitBetas(:,:,Dii));
    close()
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




disp('Smoothing individual function...')
ResYdesign = Ydesign - efitYdesign;
[ ResEtas, efitEtas, eSigEta ] = MVCM_sif( arclength_allPos, ResYdesign );
[ mSigEtaEig, mSigEta ] = MVCM_eigen( efitEtas );




%% 3. Omnibus Hypothesis Test
if( omnibus == 1 )
    disp('3. Omnibus')
    Gstats = zeros( 1, nbrCovariates-1 );
    Lstats = zeros( nbrArclengths, nbrCovariates-1 );
    Gpvals = zeros( 1, nbrCovariates-1 );
    
    disp('Calculating bias...')
    [ ebiasBetas ] = MVCM_bias( NoSetup, arclength_allPos, Xdesign, Ydesign, InvSigmats, mh );
    
    disp('Calculating omnibus individual and global statistics...')
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
    disp('Saving omnibus local p-values...')
    csvwrite(sprintf('%s/%s_%s_Omnibus_Local_pvalues.csv',savingFolder,Fnames{1},params{1}),Lpvals);  % column for each covariate; local p-values are computed at each arclength
    
    
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% plot local p values  - These p-values are NOT corrected for multiple comparisons
    figure()
    hold on
    for pii=2:nbrCovariates
        h=plot(arclength,-log10(Lpvals(:,pii-1)),'Color',color{pii-1},'LineWidth', 1.25, 'LineSmoothing', 'ON');
    end
    hold off
    
    xlabel('Arc Length','fontweight','bold');
    ylabel('-log10(p)','fontweight','bold');
    xlim([min(arclength) max(arclength)]);
    xL = get(gca,'XLim');
    line(xL,[1.3 1.3],'Color','black'); % line at 1.3 to mark significance level
    h=legend(Pnames(2:nbrCovariates),'Location','SouthEastOutside');
    title(sprintf('%s %s Local p-values',Fnames{1},params{1}),'fontweight','bold');
    clear h;
    
    %save plot
    saveas(gcf,sprintf('%s/%s_%s_Omnibus_Local_pvalues.pdf',savingFolder,Fnames{1},params{1}),'pdf');
    close()
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
    
    
    Gpvals
    disp('Saving omnibus global p-values...')
    csvwrite( sprintf( '%s/%s_%s_Omnibus_Global_pvalues.csv', savingFolder, Fnames{1}, params{1} ), Gpvals );
    
    
    disp('Correcting omnibus local p-values...')
    %% correct local p-values with FDR
    % this corrects the local p-values for multiple comparisons
    Lpvals_FDR = zeros( size( Lpvals ) );
    for pii = 2:nbrCovariates
        Lpvals_FDR( :, pii-1 ) = myFDR( Lpvals( :, pii-1 ));
    end
    
    disp('Saving omnibus FDR Local nbrCovariates-Values file...')
    csvwrite( sprintf( '%s/%s_%s_Omnibus_FDR_Local_pvalues.csv', savingFolder, Fnames{1}, params{1} ), Lpvals_FDR );
    
    
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% plot corrected local p values for all covariates
    figure()
    hold on
    for pii=2:nbrCovariates
        h=plot(arclength,-log10(Lpvals_FDR(:,pii-1)),'Color',color{pii-1},'LineWidth', 1.25, 'LineSmoothing', 'ON');
    end
    hold off
    
    xlabel('Arc Length','fontweight','bold');
    ylabel('-log10(p)','fontweight','bold');
    xlim([min(arclength) max(arclength)]);
    xL = get(gca,'XLim');
    line(xL,[1.3 1.3],'Color','black'); % line at 1.3 to mark significance level
    h=legend(Pnames(2:nbrCovariates),'Location','SouthEastOutside');
    title(sprintf('%s %s FDR Local p-values',Fnames{1},params{1}),'fontweight','bold');
    clear h;
    
    %save plot
    saveas(gcf,sprintf('%s/%s_%s_Omnibus_FDR_Local_pvalues.pdf',savingFolder,Fnames{1},params{1}),'pdf');
    close()
    
    
    %% plot corrected local p values for each covariates
    for pii=2:nbrCovariates
        figure()
        plot(arclength, -log10(Lpvals_FDR(:,pii-1)), '-', 'Color', color{pii-1}, 'LineWidth', 1.25, 'Marker', '.', 'MarkerSize',15, 'LineSmoothing', 'ON');
        
        xlabel('Arc Length','fontweight','bold');
        ylabel('-log10(p)','fontweight','bold');
        xlim([min(arclength) max(arclength)]);
        xL = get(gca,'XLim');
        line(xL,[1.3 1.3],'Color','black'); % line at 1.3 to mark significance level (0.05)
        h=legend(Pnames{pii},'Location','SouthEastOutside');
        title(sprintf('%s %s FDR %s Local p-values',Fnames{1},params{1},Pnames{pii}),'fontweight','bold');
        clear h;
        
        %save plot
        saveas(gcf,sprintf('%s/%s_%s_Omnibus_FDR_%s_Local_pvalues.pdf',savingFolder,Fnames{1},params{1},Pnames{pii}),'pdf');
        close()
    end
    
    
    %% plot ALL betas zoomed in - for a multivariate analysis - NEW way of plotting betas
    % betas are the coefficients that describe how related the covariate is to
    % the parameter
    % This is the new beta plotting from Mihye. Edited by Martin to put all covariates for each diffusion parameter on a single plot
    AlphaOM=0.05;  % Alpha level used as significance threshold for plotting Omnibus results
    
    for Dii=1:nbrDiffusionProperties % this may ned to be mii = 1:m
        figure()
        hold on
        for pii=2:nbrCovariates
            h(pii-1)=plot(arclength,efitBetas(pii,:,Dii),'Color',color{pii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
            plot(arclength(ind),efitBetas(pii,ind,Dii),'o','Color',color{pii},'LineWidth', 1.25, 'Color',color{pii}, 'LineSmoothing', 'ON');
        end
        hold off
        
        xlabel('Arc Length','fontweight','bold');
        ylabel(Dnames{Dii},'fontweight','bold');
        xlim([min(arclength) max(arclength)]);
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        legend([h(:)],Pnames(2:nbrCovariates),'Location','SouthEastOutside');
        title(sprintf('%s %s ALLcovars Beta Values',Fnames{1},Dnames{Dii}),'fontweight','bold');
        clear h;
        
        % save plot
        saveas(gcf,sprintf('%s/%s_%s_ALLcovars_Omnibus_FDR_Betas.pdf',savingFolder,Fnames{1},Dnames{Dii}),'pdf');
        close()
    end
    
    
    %% plot SIGNIFICANT BETAS only- ZOOMED IN - for ALL diffusion paramters on one plot -- nbrArclengths plot for each COVARIATE
    for pii = 2:nbrCovariates;
        figure()
        hold on
        for Dii=1:nbrDiffusionProperties
            h(Dii)=plot(arclength,efitBetas(pii,:,Dii),'-','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
            plot(arclength(ind),efitBetas(pii,ind,Dii),'o','Color',color{Dii})
            plot(arclength(ind),efitBetas(pii,ind,Dii),'*','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
        end
        hold off
        
        xlabel('Arc Length','fontweight','bold');
        ylabel('Beta Values','fontweight','bold');
        xlim([min(arclength) max(arclength)]);
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        legend([h(:)],Dnames(1:nbrDiffusionProperties),'Location','SouthEastOutside');
        title(sprintf('%s %s Significant Betas',Fnames{1},Pnames{pii}),'fontweight','bold');
        clear h;
        
        % save plot
        saveas(gcf,sprintf('%s/%s_%s_Omnibus_FDR_SigBetas.pdf',savingFolder,Fnames{1},Pnames{pii}),'pdf');
        close()
    end
    
    
    %% plot SIGNIFICANT BETAS only- Y-LIMITED - for ALL diffusion paramters on one plot -- nbrArclengths plot for each COVARIATE
    for pii = 2:nbrCovariates;
        figure()
        hold on
        for Dii=1:nbrDiffusionProperties
            h(Dii)=plot(arclength,efitBetas(pii,:,Dii),'-','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            ind=find(Lpvals_FDR(:,pii-1)<=AlphaOM);
            plot(arclength(ind),efitBetas(pii,ind,Dii),'o','Color',color{Dii})
            plot(arclength(ind),efitBetas(pii,ind,Dii),'*','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
        end
        hold off
        
        xlabel('Arc Length','fontweight','bold');
        ylabel('Beta Values','fontweight','bold');
        xlim([min(arclength) max(arclength)]);
        ylim([ylimMin ylimMax]);
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        legend([h(:)],Dnames(1:nbrDiffusionProperties),'Location','SouthEastOutside');
        title(sprintf('%s %s Significant Betas',Fnames{1},Pnames{pii}),'fontweight','bold');
        clear h;
        
        % save plot
        saveas(gcf,sprintf('%s_%s_Omnibus_FDR_SigBetas_ylimit.pdf',Fnames{1},Pnames{pii}),'pdf');
        close()
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
    
    disp('Calculating omnibus covariate confidence bands...')
    [Gvalue] = MVCM_cb_Gval( arclength_allPos, Xdesign, ResYdesign, InvSigmats, mh, GG );
    alpha = 0.05;
    [CBands] = MVCM_CBands( nbrSubjects, alpha, Gvalue, efitBetas, zeros( size( ebiasBetas ) ) );
    
    
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% plot omnibus confidence bands
    for Dii=1:nbrDiffusionProperties
        for pii=1:nbrCovariates
            figure()
            hold on
            plot(arclength,efitBetas(pii,:,Dii),'-b','LineWidth', 1.25, 'LineSmoothing', 'ON'); % Parameter (FA,MD,RD,AD)
            plot(arclength,CBands(2*pii-1,:,Dii),'--r','LineWidth', 1.25, 'LineSmoothing', 'ON'); % Lower Conf Band
            plot(arclength,CBands(2*pii,:,Dii),'--r','LineWidth', 1.25, 'LineSmoothing', 'ON'); % Upper Conf Band
            hold off
            
            xlabel('Arc Length','fontweight','bold');
            ylabel(Dnames{Dii},'fontweight','bold');
            xlim([min(arclength) max(arclength)]);
            xL = get(gca,'XLim');
            line(xL,[0 0],'Color','black'); % line at zero
            title(sprintf('95 percent confidence band for %s (%s)',Pnames{pii},Dnames{Dii}),'fontweight','bold');
            
            % save plot
            saveas(gcf,sprintf('%s/%s_%s_%s_Omnibus_confidence_band.pdf',savingFolder,Fnames{1},Dnames{Dii},Pnames{pii}),'pdf');
            close()
        end
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
    
    
end
% End of Omnibus Hypothesis Test



%% 4. Post-hoc Hypothesis Test --> Which Diffusion Parameter is significant where for each covariate? nbrArclengths univariate test in a multivariate model.
if( postHoc == 1 )
    if( omnibus == 1 )
        disp('4. Post-hoc')
    else
        disp('3. Post-hoc')
    end
    
    disp('Comparing the significance of each diffusion parameter for each covariate...')
    posthoc_Gpvals = zeros( nbrDiffusionProperties, nbrCovariates-1 );
    posthoc_Lpvals = zeros( nbrArclengths, nbrDiffusionProperties, nbrCovariates-1 );
    
    disp('Calculating post-hoc individual and global statistics...')
    for pii = 2:nbrCovariates;
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
    
    disp('Saving post-hoc global p-values...')
    csvwrite( sprintf( '%s/%s_PostHoc_Global_pvalues.csv', savingFolder, Fnames{1} ), posthoc_Gpvals );
    
    
    disp('Correcting post-hoc local p-values...')
    %% correct posthoc test local p-values with FDR
    % this corrects the posthoc local p-values for multiple comparisons
    posthoc_Lpvals_FDR = zeros( size( posthoc_Lpvals ) );
    for Dii = 1:nbrDiffusionProperties
        for pii = 2:nbrCovariates
            posthoc_Lpvals_FDR( :, Dii, pii-1 ) = myFDR( posthoc_Lpvals( :, Dii, pii-1 ) );
        end
    end
    
    
    disp('Saving post-hoc FDR local nbrCovariates-Values file...')
    for Dii = 1:nbrDiffusionProperties
        csvwrite( sprintf( '%s/%s_%s_PostHoc_Local_pvalues.csv', savingFolder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals( :, Dii, : ) );
        csvwrite( sprintf( '%s/%s_%s_PostHoc_FDR_Local_pvalues.csv', savingFolder, Fnames{1}, Dnames{Dii} ), posthoc_Lpvals_FDR( :, Dii, : ) );
    end
    
    
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%% Plotting Starts %%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% Plot CORRECTED Post-hoc test Local nbrCovariates-values for Each Difffusion Parameter
    for pii = 2:nbrCovariates;
        figure()
        hold on
        for Dii=1:nbrDiffusionProperties
            plot(arclength, -log10(posthoc_Lpvals_FDR(:,Dii,pii-1)), '-', 'Color', color{Dii}, 'LineWidth', 1.25, 'LineSmoothing', 'ON', 'Marker', '.', 'MarkerSize', 15);
        end
        hold off
        
        xlabel('Arc Length','fontweight','bold');
        ylabel('-log10(p)','fontweight','bold');
        xlim([min(arclength) max(arclength)]);
        xL = get(gca,'XLim');
        line(xL,[1.3 1.3],'Color','black'); % line at 1.3 to mark significance level
        legend([Dnames(1:nbrDiffusionProperties)],'Location','SouthEastOutside');
        title(sprintf('%s %s Posthoc FDR Local p-values',Fnames{1},Pnames{pii}),'fontweight','bold');
        
        % save local p-value plot
        saveas(gcf,sprintf('%s/%s_%s_PostHoc_FDR_Local_pvalues.pdf',savingFolder,Fnames{1},Pnames{pii}),'pdf');
        close()
    end
    
    
    %% plot CORRECTED Post-Hoc Test SIGNIFICANT BETAS only- ZOOOMED IN - for ALL diffusion paramters on one plot -- nbrArclengths plot for each COVARIATE
    
    AlphaPH=0.05;
    
    for pii = 2:nbrCovariates;
        figure()
        hold on
        for Dii=1:nbrDiffusionProperties
            h(Dii)=plot(arclength,efitBetas(pii,:,Dii),'-','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
            plot(arclength(ind),efitBetas(pii,ind,Dii),'Marker','.','MarkerSize',15)
            plot(arclength(ind),efitBetas(pii,ind,Dii),'*','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
        end
        hold off
        
        xlabel('Arc Length','fontweight','bold');
        ylabel('Beta Values','fontweight','bold');
        xlim([min(arclength) max(arclength)]);
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        legend([h(:)],Dnames(1:nbrDiffusionProperties),'Location','SouthEastOutside');
        title(sprintf('%s %s Post-Hoc Significant Betas',Fnames{1},Pnames{pii}),'fontweight','bold');
        clear h;
        
        % save plot
        saveas(gcf,sprintf('%s/%s_%s_PostHoc_FDR_SigBetas.pdf',savingFolder,Fnames{1},Pnames{pii}),'pdf');
        close()
    end
    
    
    %% plot CORRECTED Post-Hoc Test SIGNIFICANT BETAS only- Y AXIS LIMITS - for ALL diffusion paramters on one plot -- nbrArclengths plot for each COVARIATE
    for pii = 2:nbrCovariates;
        figure()
        hold on
        for Dii=1:nbrDiffusionProperties
            h(Dii)=plot(arclength,efitBetas(pii,:,Dii),'-','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
            plot(arclength(ind),efitBetas(pii,ind,Dii),'o','Color',color{Dii})
            plot(arclength(ind),efitBetas(pii,ind,Dii),'*','Color',color{Dii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
        end
        hold off
        
        xlabel('Arc Length','fontweight','bold');
        ylabel('Beta Values','fontweight','bold');
        xlim([min(arclength) max(arclength)]);
        ylim([ylimMin ylimMax]);
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        legend([h(:)],Dnames(1:nbrDiffusionProperties),'Location','SouthEastOutside');
        title(sprintf('%s %s Post-Hoc Significant Betas',Fnames{1},Pnames{pii}),'fontweight','bold');
        
        % save plot
        saveas(gcf,sprintf('%s/%s_%s_PostHoc_FDR_SigBetas_ylimit.pdf',savingFolder,Fnames{1},Pnames{pii}),'pdf');
        close()
    end
    
    
    %% plot ALL betas zoomed in - for a multivariate analysis - NEW way of plotting betas --POSTHOC CORRECTED
    % betas are the coefficients that describe how related the covariate is to
    % the parameter
    % This is the new beta plotting from Mihye. Edited by Martin (then re-edited by Jessica) to put all covariates for each diffusion parameter on a single plot
    AlphaPH=0.05;  % Alpha level used as significance threshold for plotting Omnibus results
    
    for Dii=1:nbrDiffusionProperties
        figure()
        hold on
        for pii=2:nbrCovariates
            h(pii-1)=plot(arclength,efitBetas(pii,:,Dii),'Color',color{pii},'LineWidth', 1.25, 'LineSmoothing', 'ON');
            ind=find(posthoc_Lpvals_FDR(:,Dii,pii-1)<=AlphaPH);
            plot(arclength(ind),efitBetas(pii,ind,Dii),'o','Color',color{pii},'LineWidth', 1.25, 'Color',color{pii}, 'LineSmoothing', 'ON');
            
        end
        hold off
        
        xlabel('Arc Length','fontweight','bold');
        ylabel(Dnames{Dii},'fontweight','bold');
        xlim([min(arclength) max(arclength)]);
        xL = get(gca,'XLim');
        line(xL,[0 0],'Color','black'); % line at zero
        legend([h(:)],Pnames(2:nbrCovariates),'Location','SouthEastOutside');
        title(sprintf('%s %s ALLcovars Post-Hoc Significant Estimated Coefficient',Fnames{1},Dnames{Dii}),'fontweight','bold');
        clear h;
        
        % save plot
        saveas(gcf,sprintf('%s/%s_%s_ALLcovars_PostHoc_FDR_Betas.pdf',savingFolder,Fnames{1},Dnames{Dii}),'pdf');
        close()
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%% Plotting Ends %%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    
    
    
end
% End of Post-hoc Hypothesis Test


disp('End of script')

