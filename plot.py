# 参考 https://qiita.com/lynss/items/993c2fe887fa5e1c831b
import subprocess
import plotly.graph_objs as go
import plotly.express as px 
import pandas as pd
from jinja2 import Environment, FileSystemLoader

dataset = pd.read_csv("build/output.csv")
status_data = pd.read_csv("build/status_output.csv")
print(dataset)

# make figure
fig_dict = {
    "data": [],
    "layout": {},
    "frames": []
}

range = [-1, 1]
# fig_dict["layout"]["yaxis"] = {"range": range}
fig_dict["layout"]["scene"] = dict(
    xaxis=dict(range=range),
    yaxis=dict(range=range),
    zaxis=dict(range=range),
)
fig_dict["layout"]["updatemenus"] = [
    {
        "buttons": [
            {
                "args": [None, {"frame": {"duration": 100, "redraw": True},
                                "fromcurrent": True, "transition": {"duration": 300,
                                                                    "easing": "quadratic-in-out"}}],
                "label": "&#9654;",
                "method": "animate"
            },
            {
                "args": [[None], {"frame": {"duration": 0, "redraw": False},
                                  "mode": "immediate",
                                  "transition": {"duration": 0}}],
                "label": "&#9724;",
                "method": "animate"
            }
        ],
        "direction": "left",
        "pad": {"r": 10, "t": 87},
        "showactive": False,
        "type": "buttons",
        "x": 0.1,
        "xanchor": "right",
        "y": 0,
        "yanchor": "top"
    }
]

sliders_dict = {
    "active": 0,
    "yanchor": "top",
    "xanchor": "left",
    "currentvalue": {
        "font": {"size": 20},
        "prefix": "t:",
        "visible": True,
        "xanchor": "right"
    },
    "transition": {"duration": 300, "easing": "cubic-in-out"},
    "pad": {"b": 10, "t": 50},
    "len": 0.9,
    "x": 0.1,
    "y": 0,
    "steps": []
}

# make data
t = 0
dataset_by_t = dataset[dataset["t"] == t]

data_dict = go.Scatter3d(
    x = dataset_by_t["x"],
    y = dataset_by_t["y"],
    z = dataset_by_t["z"],
    mode = "markers",
    marker = {
        "size": 2,
    },
)
data_dict = dict(type="scatter", x = dataset["x"], y = dataset["y"])
fig_dict["data"].append(data_dict)

# make frames
for t in list(dict.fromkeys(dataset["t"])):
    frame = {"data": [], "name": str(t)}
    dataset_by_t = dataset[dataset["t"] == t]

    data_dict = go.Scatter3d(
        x = dataset_by_t["x"],
        y = dataset_by_t["y"],
        z = dataset_by_t["z"],
        mode= "markers",
        marker= {
            "size": 2,
        },
    )
    frame["data"].append(data_dict)

    fig_dict["frames"].append(frame)
    slider_step = {
      "args": [
        [t],
        {"frame": {"duration": 300, "redraw": True},
         "mode": "immediate",
         "transition": {"duration": 300}}
      ],
      "label": t,
      "method": "animate"
    }
    sliders_dict["steps"].append(slider_step)

fig_dict["layout"]["sliders"] = [sliders_dict]

fig1 = go.Figure(fig_dict)
fig2 = go.Figure(px.line(status_data, x="t", y="total_energy", range_y = [-3, 0]))
# fig2 = go.Figure(go.Scatter(x=status_data["t"], y=status_data["total_energy"], mode="lines", yrange=[-3, 0]))
fig2.update_xaxes(fixedrange=True)

env = Environment(loader=FileSystemLoader('./'))
tpl = env.get_template('dashboard.html.jinja')
html = tpl.render(main_fig=fig1.to_html(full_html=False), sub_fig=fig2.to_html(full_html=False))
filename="build/dashboard.html"
dashboard = open(filename, 'w')
dashboard.write(html)
subprocess.run("xdg-open " + filename, shell = True)