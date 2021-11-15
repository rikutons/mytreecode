# 参考 https://qiita.com/lynss/items/993c2fe887fa5e1c831b
import plotly.express as px
import pandas as pd
data = pd.read_csv("build/output.csv")
print(data)
range = [-1, 1]

fig = px.scatter_3d(data, x="x", y="y",z="z", animation_frame="t", animation_group="id",
           range_x=range, range_y=range, range_z=range) 

fig.show()