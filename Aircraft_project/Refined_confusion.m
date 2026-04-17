%% 1) Load model and data
Mdl = loadLearnerForCoder('DecisionTreeModel');   % your tree model

% predictors (adjust names to match your table)
X = T{:, {'qCmd','qMeas','force','Force'}};

% true labels (0–3 or 0–4)
YTrue = T.Event;          % <-- change 'Label' to your true‑label variable name

%% 2) Predict labels for all samples
YPred = predict(Mdl, X);

%% After filtering out class 0

YTrue = categorical(YTrue);
YPred = categorical(YPred);

dropClass = categorical("0");

mask   = (YTrue ~= dropClass);
YTrue3 = YTrue(mask);
YPred3 = YPred(mask);

maskPred = (YPred3 ~= dropClass);
YTrue3   = YTrue3(maskPred);
YPred3   = YPred3(maskPred);

% Remove any unused categories (including '0') so no empty rows/cols
YTrue3 = removecats(YTrue3);    % drops all unused categories[web:110]
YPred3 = removecats(YPred3);

%% Confusion matrix and 2‑panel chart
[C3, order3] = confusionmat(YTrue3, YPred3);

figure;
cm = confusionchart(YTrue3, YPred3, ...
    'Normalization', 'row-normalized', ...
    'RowSummary', 'row-normalized', ...
    'ColumnSummary', 'off');    % only main matrix + TPR/FNR panel[web:2]

cm.Title  = 'Confusion Matrix (3 Classes)';
cm.XLabel = 'Predicted Class';
cm.YLabel = 'True Class';

