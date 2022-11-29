from fairlearn.metrics import MetricFrame

from sklearn.metrics import accuracy_score

from sklearn.tree import DecisionTreeClassifier
>>>

classifier = DecisionTreeClassifier(min_samples_leaf=10, max_depth=4)

classifier.fit(X, y_true)
DecisionTreeClassifier(...)

y_pred = classifier.predict(X)

gm = MetricFrame(metrics=accuracy_score, y_true=y_true, y_pred=y_pred, sensitive_features=sex)

print(gm.overall)
0.8443...

print(gm.by_group)
sex
Female    0.9251...
Male      0.8042...
Name: accuracy_score, dtype: float64