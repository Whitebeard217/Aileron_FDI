% Specify the Excel file and the column name (or number)
filename = 'Fault_marker_data.xlsx';  % Update this with your file name
sheet = 1;                  % Optional: specify the sheet (or leave as default)
columnName = 'Detected_fault';   % Update with your column name

% Read the data from the Excel file
data = readtable(filename, 'Sheet', sheet);

% Extract the selected column
colData = data.(columnName);

% Find the unique classes
[classes, ~, idx] = unique(colData);

% Count the number of samples in each class
counts = histcounts(idx, numel(classes));

% Display the results
fprintf('Number of unique classes: %d\n\n', numel(classes));
disp('Class           Count');
for i = 1:numel(classes)
    fprintf('%-15s %d\n', string(classes(i)), counts(i));
end