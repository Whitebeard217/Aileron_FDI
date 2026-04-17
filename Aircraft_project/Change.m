data = readtable('Event_Data.xlsx');
class_column = data{:, end};
class_column(class_column == 4) = 1;
class_column(class_column == 3) = 2;
class_column(class_column == 5 | class_column == 6) = 3;
data{:, end} = class_column;
writetable(data, 'updated_Event_Data.xlsx');