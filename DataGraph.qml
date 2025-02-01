import QtQuick
import QtQuick.Controls

Item {
    property DataAxis xaxis
    property list<DataAxis> yaxis
    property list<DataSeries> series
    property string title: "Graph"
    property int textPadding: 5
    property int topMargin: 75
    property int bottomMargin: 75
    property int leftMargin: 50
    property int rightMargin: 50
    property real shiftX: 0
    property real shiftY: 0
    property real zoom: 1
    property bool autoScale: true
    id: root

    // Store dynamically created CB objects
    property var checkboxes: []
    // Store dynamically created MouseArea objects
    property var series_clicks: []
    property int series_highlighted: -1

    // dynamic bottom_offset from legend
    property int bottom_off: 0

    Canvas {
        id: canvas
        anchors.fill: parent
        antialiasing: true

        function truncateToSignificantFigures(num, significantFigures) {
            if (num === 0) return 0; // Special case for 0

            const factor = Math.pow(10, Math.floor(Math.log10(Math.abs(num))) - (significantFigures - 1));
            return Math.floor(num / factor) * factor;
        }

        function get_label_for_number(val) {
            if(val === 0) {
                return "0"
            } else if(Math.abs(val) < 0.01) {
                return val.toExponential(2)
            } else if (Math.abs(val) < 10) {
                return parseFloat(val.toFixed(2));
            } else if(Math.abs(val) >= 100000) {
                return val.toExponential(2)
            } else {
                return val.toFixed(0)
            }
        }

        onPaint: {
            if(xaxis == null || yaxis == null || yaxis.length < 1) {
                return
            }

            var ctx = getContext("2d")

            // Reset Canvas
            ctx.reset()

            // Draw legend
            ctx.textBaseline = "middle"
            ctx.textAlign = "right"
            ctx.font = "12px sans-serif"
            ctx.fillStyle = Qt.rgba(1, 1, 1, 1)
            ctx.lineWidth = 3
            // First find the required width
            var legend_width = [[0, 0]] // First element - width, second - number of series names
            var legend_idx = 0
            for(var i = 0; i < series.length; i++) {
                let sname = series[i].name
                legend_width[legend_idx][0] += ctx.measureText(sname).width + 60 + textPadding
                legend_width[legend_idx][1] += 1
                // If its too wide, go to a new line
                if(legend_width[legend_idx][0] > width-rightMargin-leftMargin) {
                    legend_width.push([0, 0])
                    legend_idx += 1
                }
            }

            // Then draw legend, bottom up
            var legend_x = leftMargin + ((width-rightMargin-leftMargin)/2) + (legend_width[legend_idx][0]/2)
            var legend_y = height - bottomMargin - 6 // bottom margin and half of font height
            for(i = series.length-1; i >= 0; i--) {
                // Go to previous line if needed
                if(legend_width[legend_idx][1] <= 0) {
                    legend_idx -= 1
                    if(legend_idx >= 0) {
                        legend_x = leftMargin + ((width-rightMargin-leftMargin)/2) + (legend_width[legend_idx][0]/2)
                        legend_y -= 16
                    }
                }
                let sname = series[i].name
                ctx.beginPath()
                ctx.strokeStyle = series[i].color
                ctx.fillText(sname, legend_x, legend_y)
                // Start placing the click box too
                series_clicks[i].width = legend_x + 4
                series_clicks[i].height = 16
                series_clicks[i].y = legend_y - 8
                legend_x -= ctx.measureText(sname).width + textPadding
                ctx.moveTo(legend_x, legend_y)
                ctx.lineTo(legend_x - 20, legend_y)
                ctx.stroke()
                ctx.closePath()
                // Finish click box
                series_clicks[i].width -= (legend_x - 22)
                series_clicks[i].x = legend_x - 22
                legend_x -= 50
                // Position checkbox
                if(checkboxes[i]) {
                    checkboxes[i].x = legend_x
                    checkboxes[i].y = legend_y - checkboxes[i].height/2
                }
                legend_x -= 10

                legend_width[legend_idx][1] -= 1
            }
            ctx.lineWidth = 1

            // Dynamic based on the legend size
            bottom_off = height - (legend_y - 6 - textPadding) + 12 + 16 + 3*textPadding

            // Get axis ranges and spacing
            var xmin = (xaxis.min + Math.floor(shiftX*(xaxis.max-xaxis.min)/(height-bottom_off-topMargin))) * zoom
            var xmax = (xaxis.max + Math.floor(shiftX*(xaxis.max-xaxis.min)/(height-bottom_off-topMargin))) * zoom
            var scale_graph_to_window_x = (width-leftMargin-rightMargin)/(xmax-xmin)
            var ymin = (yaxis[0].min + Math.floor(shiftY*(yaxis[0].max-yaxis[0].min)/(height-bottom_off-topMargin))) * zoom
            var ymax = (yaxis[0].max + Math.floor(shiftY*(yaxis[0].max-yaxis[0].min)/(height-bottom_off-topMargin))) * zoom
            var scale_graph_to_window_y = (height-bottom_off-topMargin)/(ymax-ymin)
            var gridSpacingX = truncateToSignificantFigures((xmax-xmin)/xaxis.ticks, 2)
            var gridSpacingY = truncateToSignificantFigures((ymax-ymin)/yaxis[0].ticks, 2)

            // Draw gridlines and tick labels
            ctx.textBaseline = "middle"
            ctx.font = "12px sans-serif"
            ctx.fillStyle = Qt.rgba(1, 1, 1, 1)

            var grid_spacing_window_x = gridSpacingX * scale_graph_to_window_x
            var grid_spacing_window_y = gridSpacingY * scale_graph_to_window_y

            // Find first gridlines that can appear
            var gridx = Math.ceil(xmin/gridSpacingX) * gridSpacingX
            var grid_pos_vert = (gridx-xmin) * scale_graph_to_window_x + leftMargin
            var gridy = Math.ceil(ymin/gridSpacingY) * gridSpacingY
            var grid_pos_horiz = (height - bottom_off) - (gridy-ymin) * scale_graph_to_window_y

            ctx.beginPath()
            ctx.strokeStyle = "#666666"
            ctx.textBaseline = "top"
            ctx.textAlign = "center"
            while(true) {
                if(grid_spacing_window_x == 0) {
                    break
                }
                if(grid_pos_vert > width-rightMargin) {
                    break
                }

                // Draw gridline
                ctx.moveTo(grid_pos_vert, height-bottom_off)
                ctx.lineTo(grid_pos_vert, topMargin)

                // Fill tick label
                ctx.fillText(get_label_for_number(gridx), grid_pos_vert, height-bottom_off + textPadding)

                grid_pos_vert += grid_spacing_window_x
                gridx += gridSpacingX
            }
            ctx.textBaseline = "middle"
            ctx.textAlign = "right"
            var max_text_width = 0
            while(true) {
                if(grid_spacing_window_y == 0) {
                    break
                }
                if(grid_pos_horiz < topMargin) {
                    break
                }

                // Draw gridline
                ctx.moveTo(leftMargin, grid_pos_horiz)
                ctx.lineTo(width-rightMargin, grid_pos_horiz)

                // Fill tick label and record maximum width
                ctx.fillText(get_label_for_number(gridy), leftMargin - textPadding, grid_pos_horiz)

                var text_width = ctx.measureText(get_label_for_number(gridy)).width
                if(text_width > max_text_width) {
                    max_text_width = text_width
                }

                grid_pos_horiz -= grid_spacing_window_y
                gridy += gridSpacingY
            }
            ctx.stroke()
            ctx.closePath()

            // Add x axis label
            ctx.font = "16px sans-serif"
            ctx.textAlign = "center"
            ctx.textBaseline = "top"
            ctx.fillText(xaxis.label, (width - rightMargin - leftMargin)/2 + leftMargin, height - bottom_off + textPadding*2 + 12) // Account for height of tick labels

            // Add y axis label
            ctx.font = "16px sans-serif"
            ctx.textAlign = "center"
            ctx.textBaseline = "bottom"
            ctx.translate(leftMargin - 2*textPadding - max_text_width, (height - bottom_off - topMargin)/2 + topMargin)
            ctx.rotate(-Math.PI/2)
            ctx.fillText(yaxis[0].label, 0, 0)
            ctx.setTransform(1, 0, 0, 1, 0, 0);

            // Draw series
            for(i = 0; i < series.length; i++) {
                // Check whether user has selected series to be displayed
                if(checkboxes[i] === null || checkboxes[i].checked === false) {
                    continue
                }

                ctx.strokeStyle = series[i].color
                ctx.beginPath()

                // Use the proper y-axis
                ymin = (series[i].axisy.min + Math.floor(shiftY*(series[i].axisy.max-series[i].axisy.min)/(height-bottom_off-topMargin))) * zoom
                ymax = (series[i].axisy.max + Math.floor(shiftY*(series[i].axisy.max-series[i].axisy.min)/(height-bottom_off-topMargin))) * zoom
                scale_graph_to_window_y = (height-bottom_off-topMargin)/(ymax-ymin)

                for(let j = 1; j < series[i].data.length; j++) {
                    var line_start_x = series[i].data[j-1].x
                    var line_start_y = series[i].data[j-1].y
                    var line_end_x = series[i].data[j].x
                    var line_end_y = series[i].data[j].y

                    // Discard lines outside of the graph
                    if(line_start_x < xmin && line_end_x < xmin) {
                        continue
                    }
                    if(line_start_y < ymin && line_end_y < ymin) {
                        continue
                    }
                    if(line_start_x > xmax && line_end_x > xmax) {
                        continue
                    }
                    if(line_start_y > ymax && line_end_y > ymax) {
                        continue
                    }

                    let slope = (line_end_y - line_start_y)/(line_end_x - line_start_x)

                    //console.log(xmin, xmax, ymin, ymax)

                    // Advance line start to meet graph edge
                    if(line_start_x < xmin) {
                        let dx = xmin - line_start_x
                        line_start_x += dx
                        line_start_y += dx*slope
                    }
                    if(line_start_y < ymin) {
                        let dy = ymin - line_start_y
                        line_start_y += dy
                        line_start_x += dy/slope
                    }
                    if(line_end_x < xmin) {
                        let dx = xmin - line_end_x
                        line_end_x += dx
                        line_end_y += dx*slope
                    }
                    if(line_end_y < ymin) {
                        let dy = ymin - line_end_y
                        line_end_y += dy
                        line_end_x += dy/slope
                    }

                    //console.log(line_start_x, line_end_x, line_start_y, line_end_y)

                    // Retract line end to meet graph edge
                    if(line_end_x > xmax) {
                        let dx = xmax - line_end_x
                        line_end_x += dx
                        line_end_y += dx*slope
                    }
                    if(line_end_y > ymax) {
                        let dy = ymax - line_end_y
                        line_end_y += dy
                        line_end_x += dy/slope
                    }
                    if(line_start_x > xmax) {
                        let dx = xmax - line_start_x
                        line_start_x += dx
                        line_start_y += dx*slope
                    }
                    if(line_start_y > ymax) {
                        let dy = ymax - line_start_y
                        line_start_y += dy
                        line_start_x += dy/slope
                    }

                    //console.log(slope)

                    // Reframe to the graph
                    line_start_x = (line_start_x-xmin)*scale_graph_to_window_x + leftMargin
                    line_end_x = (line_end_x-xmin)*scale_graph_to_window_x + leftMargin
                    line_start_y = height - bottom_off - (line_start_y-ymin)*scale_graph_to_window_y
                    line_end_y = height - bottom_off - (line_end_y-ymin)*scale_graph_to_window_y

                    slope = (line_end_y - line_start_y)/(line_end_x - line_start_x)
                    //console.log(slope)

                    ctx.moveTo(line_start_x, line_start_y)
                    ctx.lineTo(line_end_x, line_end_y)
                }
                ctx.stroke()
                ctx.closePath()
            }

            // Draw auxiliary axes
            ctx.strokeStyle = "#AAAAAA"
            ctx.beginPath()
            var axis_label_shift = 0
            for(i = 1; i < yaxis.length; i++) {
                ymin = (yaxis[i].min + Math.floor(shiftY*(yaxis[i].max-yaxis[i].min)/(height-bottom_off-topMargin))) * zoom
                ymax = (yaxis[i].max + Math.floor(shiftY*(yaxis[i].max-yaxis[i].min)/(height-bottom_off-topMargin))) * zoom
                scale_graph_to_window_y = (height-bottom_off-topMargin)/(ymax-ymin)
                gridSpacingY = truncateToSignificantFigures((ymax-ymin)/yaxis[i].ticks, 2)
                grid_spacing_window_y = gridSpacingY * scale_graph_to_window_y

                // Find first ticks that can appear
                gridy = Math.ceil(ymin/gridSpacingY) * gridSpacingY
                grid_pos_horiz = (height - bottom_off) - (gridy-ymin) * scale_graph_to_window_y

                ctx.fillStyle = yaxis[i].color
                max_text_width = 0

                // Base x offest
                var base_x_off = width - rightMargin + 5*textPadding*i + axis_label_shift

                // Add divider line
                ctx.moveTo(base_x_off, topMargin)
                ctx.lineTo(base_x_off, height - bottom_off)

                // Draw tick labels
                while(true) {
                    if(grid_spacing_window_y == 0) {
                        break
                    }
                    if(grid_pos_horiz < topMargin) {
                        break
                    }

                    // Add tick mark
                    ctx.moveTo(base_x_off, grid_pos_horiz)
                    ctx.lineTo(base_x_off + textPadding, grid_pos_horiz)

                    // Add number label and save the text width for spacing use
                    ctx.font = "12px sans-serif"
                    ctx.textBaseline = "middle"
                    ctx.textAlign = "left"
                    ctx.fillText(get_label_for_number(gridy), base_x_off + 2*textPadding, grid_pos_horiz)
                    text_width = ctx.measureText(get_label_for_number(gridy)).width
                    if(text_width > max_text_width) {
                        max_text_width = text_width
                    }

                    grid_pos_horiz -= grid_spacing_window_y
                    gridy += gridSpacingY
                }

                // Add axis label
                ctx.font = "16px sans-serif"
                ctx.textAlign = "center"
                ctx.textBaseline = "middle"
                ctx.translate(base_x_off + 5*textPadding + max_text_width, (height - bottom_off - topMargin)/2 + topMargin)
                ctx.rotate(Math.PI/2)
                ctx.fillText(yaxis[i].label, 0, 0)
                ctx.setTransform(1, 0, 0, 1, 0, 0);

                axis_label_shift += max_text_width + 16 // Account for label height
            }
            ctx.stroke()
            ctx.closePath()

            // Draw frame
            ctx.strokeStyle = "#FFFFFF"
            ctx.beginPath()
            ctx.moveTo(leftMargin, topMargin)
            ctx.lineTo(width-rightMargin, topMargin)
            ctx.lineTo(width-rightMargin, height-bottom_off)
            ctx.lineTo(leftMargin, height-bottom_off)
            ctx.lineTo(leftMargin, topMargin)
            ctx.stroke()
            ctx.closePath()

            // Draw title
            ctx.textBaseline = "middle"
            ctx.textAlign = "center"
            ctx.font = "32px sans-serif"
            ctx.fillStyle = Qt.rgba(1, 1, 1, 1)
            ctx.fillText(title, leftMargin + (width-leftMargin-rightMargin)/2, topMargin/2)
        }
    }

    function scrollLeft(pixels) {
        if(xaxis == null || yaxis == null || yaxis.length < 1) {
            return
        }
        var lastShiftX = shiftX
        shiftX -= pixels
        if(Math.floor(lastShiftX) - Math.floor(shiftX) != 0) {
            canvas.requestPaint()
        }
    }

    function scrollRight(pixels) {
        scrollLeft(-1*pixels)
    }

    function scrollDown(pixels) {
        if(xaxis == null || yaxis == null || yaxis.length < 1) {
            return
        }
        var lastShiftY = shiftY
        shiftY -= pixels
        if(Math.floor(lastShiftY) - Math.floor(shiftY) != 0) {
            canvas.requestPaint()
        }
    }

    function scrollUp(pixels) {
        scrollDown(-1*pixels)
    }

    function zoomIn(x, y) {
        // rescale mouse pos to graph pos
        const range_x = xaxis.max - xaxis.min
        const graph_x = x*(range_x)/(width-leftMargin-rightMargin) + xaxis.min
        xaxis.min = graph_x - (range_x/(2*1.2))   // Zoom 1.2, centered around mouse pointer
        xaxis.max = graph_x + (range_x/(2*1.2))
        for(var i = 0; i < yaxis.length; i++) {
            const range_y = yaxis[i].max - yaxis[i].min
            const graph_y = yaxis[i].max - y*(range_y)/(height-topMargin-bottom_off)
            yaxis[i].min = graph_y - (range_y/(2*1.2))   // Zoom 1.2, centered around mouse pointer
            yaxis[i].max = graph_y + (range_y/(2*1.2))
        }

        canvas.requestPaint()
    }

    function zoomOut(x, y) {
        // rescale mouse pos to graph pos
        const range_x = xaxis.max - xaxis.min
        const graph_x = xaxis.max - x*(range_x)/(width-leftMargin-rightMargin)
        xaxis.min = graph_x - (range_x*(0.5*1.2))   // Zoom 1.2, centered around mouse pointer
        xaxis.max = graph_x + (range_x*(0.5*1.2))
        for(var i = 0; i < yaxis.length; i++) {
            const range_y = yaxis[i].max - yaxis[i].min
            const graph_y = y*(range_y)/(height-topMargin-bottom_off) + yaxis[i].min
            yaxis[i].min = graph_y - (range_y*(0.5*1.2))   // Zoom 1.2, centered around mouse pointer
            yaxis[i].max = graph_y + (range_y*(0.5*1.2))
        }

        canvas.requestPaint()
    }

    function getSeries(name) {
        for(var i = 0; i < series.length; i++) {
            if(series[i].name === name) {
                return series[i]
            }
        }
        return null
    }

    Component.onCompleted: {
        // Create checkboxes and mouse areas for series
        for(var i = 0; i < series.length; i++) {
            // Connect series to graph
            series[i].pointAdded.connect(canvas.requestPaint)
            series[i].chart = root

            // Add checkboxes
            const newCheck = Qt.createQmlObject(`
                import QtQuick.Controls

                CheckBox {
                    id: cb_`+i+`
                    checked: true
                    onToggled: parent.repaint()
                }
                `,
                root,
                "myDynamicSnippet"
            );
            checkboxes[i] = newCheck
            checkboxes[i].checked = series[i].startChecked

            // Add mouse areas
            const newMouseArea = Qt.createQmlObject(`
                import QtQuick

                Rectangle {
                    z: -1
                    color: (mouse_`+i+`.containsMouse ? ((series_highlighted === `+i+`) ? "gray" : "#3b3b3b") : ((series_highlighted === `+i+`) ? "dimgray" : "transparent"))
                    MouseArea {
                        id: mouse_`+i+`
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            if(series_highlighted === `+i+`) {
                                series_highlighted = -1
                            } else {
                                series_highlighted = `+i+`
                            }
                            parent.parent.repaint()
                        }
                    }
                }
                `,
                root,
                "myDynamicSnippet"
            );
            series_clicks[i] = newMouseArea
        }
    }

    function rescaleAxis(axis) {
        // Scale all the way down the first time, then only up
        var first_scale_x = true
        var first_scale_y = true

        // Reset translation and zoom
        zoom = 1
        shiftX = 0
        shiftY = 0

        // Loop through series
        for(var i = 0; i < series.length; i++) {
            if(isNaN(series[i].xmin) || isNaN(series[i].xmax) || isNaN(series[i].ymin) || isNaN(series[i].ymax)) {
                continue
            }
            if(series[i].xmin === series[i].xmax || series[i].ymin === series[i].ymax) {
                continue
            }

            // Check the series uses this as an x axis
            if(series[i].axisx === axis) {
                if(first_scale_x) {
                    axis.min = series[i].xmin
                    axis.max = series[i].xmax
                    first_scale_x = false
                } else {
                    if(series[i].xmin < axis.min) {
                        axis.min = series[i].xmin
                    }
                    if(series[i].xmax > axis.max) {
                        axis.max = series[i].xmax
                    }
                }
            }
            // Check the series uses this as a y axis
            if(series[i].axisy === axis) {
                if(first_scale_y) {
                    axis.min = series[i].ymin
                    axis.max = series[i].ymax
                    first_scale_y = false
                } else {
                    if(series[i].ymin < axis.min) {
                        axis.min = series[i].ymin
                    }
                    if(series[i].ymax > axis.max) {
                        axis.max = series[i].ymax
                    }
                }
            }
        }
    }

    function repaint() {
        canvas.requestPaint()
    }

    // Provide mouse area for chart interaction
    MouseArea {
        id: chart_mouse
        property int dStartX: 0
        property int dStartY: 0
        hoverEnabled: true

        x: leftMargin
        y: topMargin
        width: root.width-leftMargin-rightMargin
        height: root.height-topMargin-bottom_off

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            dStartX = chart_mouse.mouseX
            dStartY = chart_mouse.mouseY
        }

        function moveAxes() {
            if(containsPress && (pressedButtons & Qt.LeftButton)) {
                autoScale = false
                let dx = chart_mouse.mouseX - dStartX
                let dy = chart_mouse.mouseY - dStartY
                if(dx) {
                    dStartX = chart_mouse.mouseX
                }
                if(dy) {
                    dStartY = chart_mouse.mouseY
                }
                root.scrollLeft(dx)
                root.scrollUp(dy)
            }
        }

        onMouseXChanged: {
            moveAxes()
            dStartX = chart_mouse.mouseX
            dStartY = chart_mouse.mouseY
        }

        onMouseYChanged: {
            moveAxes()
            dStartX = chart_mouse.mouseX
            dStartY = chart_mouse.mouseY
        }

        onClicked: function(mouse) {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }

        onWheel: function(wheel) {
            autoScale = false
            if(wheel.angleDelta.y < 0) {
                root.zoomOut(chart_mouse.mouseX, chart_mouse.mouseY)
            } else {
                root.zoomIn(chart_mouse.mouseX, chart_mouse.mouseY)
            }
        }

        Menu {
            id: contextMenu
            MenuItem {
                text: "Autoscale Axes"
                onTriggered: {
                    root.autoScale = true
                    root.rescaleAxis(root.xaxis)
                    for(var i = 0; i < root.yaxis.length; i++) {
                        root.rescaleAxis(root.yaxis[i])
                    }
                    root.repaint()
                }
            }
        }

        Text {
            x: chart_mouse.mouseX + 2
            y: chart_mouse.mouseY - height - 2
            text: (root.series_highlighted > -1) ? ((canvas.get_label_for_number(series[root.series_highlighted].axisx.min + ((series[root.series_highlighted].axisx.max - series[root.series_highlighted].axisx.min) / chart_mouse.width) * (chart_mouse.mouseX))) +
                                                   ", " +
                                                   (canvas.get_label_for_number(series[root.series_highlighted].axisy.min + ((series[root.series_highlighted].axisy.max - series[root.series_highlighted].axisy.min) / chart_mouse.height) * (chart_mouse.height - chart_mouse.mouseY))))
                                                    : ""
            color: "white"
            Rectangle {
                anchors.fill: parent
                anchors.margins: -2
                color: (root.series_highlighted > -1) ? "dimgray" : "transparent"
                z: -1
            }
        }
    }
}
