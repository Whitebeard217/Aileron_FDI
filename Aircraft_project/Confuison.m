filename = 'Fault_marker_data.xlsx';  % Update this with your file name
sheet = 1;                  % Optional: specify the sheet (or leave as default)
% Read the Excel file (assumes two columns: Actual, Predicted)
T = readtable(filename, 'Sheet', sheet);
% Extract and clean labels
% Extract actual and predicted labels
actual = string(T{:,1});
predicted = string(T{:,2});
valid = ~ismissing(actual) & ~ismissing(predicted);
actual = actual(valid);
predicted = predicted(valid);

% Define class labels (ensure they match your real case)
classLabels = unique([actual; predicted], 'stable');

% Compute confusion matrix (raw)
CM = confusionmat(actual, predicted, 'Order', classLabels);

% Normalize rows for percentage
CMp = 100 * (CM ./ sum(CM,2));  % Percent, row-normalized

% Compute TPR (Recall) and FNR (Miss)
TPR = diag(CM) ./ sum(CM,2);  % Recall
FNR = 1 - TPR;                % Miss
TPR_FNR = [TPR, FNR];
TPR_FNR_pct = 100*TPR_FNR;

% Create matrix for display: [CMp | TPR_FNR_pct]
displayMatrix = [CMp, TPR_FNR_pct];

% Create full label lists for columns
%heatmapLabels = [cellstr(classLabels); {'Recall (TPR)', 'Miss (FNR)'}];

% Plot as heatmap
figure;
h = heatmap([cellstr(classLabels).' {'Recall (TPR)','Miss (FNR)'}], cellstr(classLabels), ...
          round(displayMatrix,1), ...
          'CellLabelFormat','%.1f%%', ...
          'Colormap',parula, ...
          'ColorbarVisible','on');
h.Title = 'Confusion Matrix with Recall (TPR) and Miss (FNR) (%)';
h.XLabel = 'Predicted class / Summary';
h.YLabel = 'Actual class';

% Optionally adjust figure size for clarity
set(gcf,'Position',get(gcf,'Position') + [0 0 100 0]);

% Display exact values in console as well
disp('Confusion Matrix with TPR (Recall) and FNR (Miss) [%]:');
displayTable = array2table(displayMatrix, ...
    'VariableNames',[cellstr(classLabels).' {'TPR_percent','FNR_percent'}], ...
    'RowNames',cellstr(classLabels));
disp(displayTable)