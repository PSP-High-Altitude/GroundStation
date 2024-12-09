import QtQuick

Item {
    property DataAxis xaxis
    property list<DataAxis> yaxis
    property list<DataSeries> series
    property string title: "Graph"
    property int textPadding: 5
    property int topMargin: 75
    property int bottomMargin: 50
    property int leftMargin: 50
    property int rightMargin: 50
    property real shiftX: 0
    property real shiftY: 0
    property real zoom: 1
    id: root

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
            } else if(Math.abs(val) > 10000) {
                return val.toExponential(2)
            } else {
                return val.toFixed(0)
            }
        }

        onPaint: {
            if(xaxis == null || yaxis == null || yaxis.length < 1) {
                return
            }

            // Get axis ranges and spacing
            var xmin = (xaxis.min + Math.floor(shiftX)) * zoom
            var xmax = (xaxis.max + Math.floor(shiftX)) * zoom
            var ymin = (yaxis[0].min + Math.floor(shiftY)) * zoom
            var ymax = (yaxis[0].max + Math.floor(shiftY)) * zoom
            var gridSpacingX = truncateToSignificantFigures((xmax-xmin)/xaxis.ticks, 2)
            var gridSpacingY = truncateToSignificantFigures((ymax-ymin)/yaxis[0].ticks, 2)

            var ctx = getContext("2d")

            // Background
            ctx.fillStyle = Qt.rgba(1, 1, 1, 1)
            ctx.fillRect(0, 0, width, height)

            // Draw gridlines and tick labels
            ctx.textBaseline = "middle"
            ctx.font = "12px sans-serif"
            ctx.fillStyle = Qt.rgba(0, 0, 0, 1)
            var scale_graph_to_window_x = (width-leftMargin-rightMargin)/(xmax-xmin)
            var grid_spacing_window_x = gridSpacingX * scale_graph_to_window_x
            var scale_graph_to_window_y = (height-bottomMargin-topMargin)/(ymax-ymin)
            var grid_spacing_window_y = gridSpacingY * scale_graph_to_window_y

            // Find first gridlines that can appear
            var gridx = Math.ceil(xmin/gridSpacingX) * gridSpacingX
            var grid_pos_vert = (gridx-xmin) * scale_graph_to_window_x + leftMargin
            var gridy = Math.ceil(ymin/gridSpacingY) * gridSpacingY
            var grid_pos_horiz = (height - bottomMargin) - (gridy-ymin) * scale_graph_to_window_y

            ctx.beginPath()
            ctx.strokeStyle = "#AAAAAA"
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
                ctx.moveTo(grid_pos_vert, height-bottomMargin)
                ctx.lineTo(grid_pos_vert, topMargin)

                // Fill tick label
                ctx.fillText(get_label_for_number(gridx), grid_pos_vert, height-bottomMargin + textPadding)

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
            ctx.fillText(xaxis.label, (width - rightMargin - leftMargin)/2 + leftMargin, height - bottomMargin + textPadding*2 + 12) // Account for height of tick labels

            // Add y axis label
            ctx.font = "16px sans-serif"
            ctx.textAlign = "center"
            ctx.textBaseline = "bottom"
            ctx.translate(leftMargin - 2*textPadding - max_text_width, (height - bottomMargin - topMargin)/2 + topMargin)
            ctx.rotate(-Math.PI/2)
            ctx.fillText(yaxis[0].label, 0, 0)
            ctx.setTransform(1, 0, 0, 1, 0, 0);

            // Draw series
            for(var i = 0; i < series.length; i++) {
                ctx.strokeStyle = series[i].color
                ctx.beginPath()

                // Use the proper y-axis
                ymin = (series[i].axisy.min + Math.floor(shiftY)) * zoom
                ymax = (series[i].axisy.max + Math.floor(shiftY)) * zoom
                scale_graph_to_window_y = (height-bottomMargin-topMargin)/(ymax-ymin)

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
                    line_start_y = height - bottomMargin - (line_start_y-ymin)*scale_graph_to_window_y
                    line_end_y = height - bottomMargin - (line_end_y-ymin)*scale_graph_to_window_y

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
                ymin = (yaxis[i].min + Math.floor(shiftY)) * zoom
                ymax = (yaxis[i].max + Math.floor(shiftY)) * zoom
                scale_graph_to_window_y = (height-bottomMargin-topMargin)/(ymax-ymin)
                gridSpacingY = truncateToSignificantFigures((ymax-ymin)/yaxis[i].ticks, 2)
                grid_spacing_window_y = gridSpacingY * scale_graph_to_window_y

                // Find first ticks that can appear
                gridy = Math.ceil(ymin/gridSpacingY) * gridSpacingY
                grid_pos_horiz = (height - bottomMargin) - (gridy-ymin) * scale_graph_to_window_y

                ctx.fillStyle = yaxis[i].color
                max_text_width = 0

                // Base x offest
                var base_x_off = width - rightMargin + 5*textPadding*i + axis_label_shift

                // Add divider line
                ctx.moveTo(base_x_off, topMargin)
                ctx.lineTo(base_x_off, height - bottomMargin)

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
                ctx.stroke()
                ctx.closePath()

                // Add axis label
                ctx.font = "16px sans-serif"
                ctx.textAlign = "center"
                ctx.textBaseline = "middle"
                ctx.translate(base_x_off + 5*textPadding + max_text_width, (height - bottomMargin - topMargin)/2 + topMargin)
                ctx.rotate(Math.PI/2)
                ctx.fillText(yaxis[i].label, 0, 0)
                ctx.setTransform(1, 0, 0, 1, 0, 0);

                axis_label_shift += max_text_width + 16 // Account for label height
            }

            // Draw frame
            ctx.strokeStyle = "#000000"
            ctx.beginPath()
            ctx.moveTo(leftMargin, topMargin)
            ctx.lineTo(width-rightMargin, topMargin)
            ctx.lineTo(width-rightMargin, height-bottomMargin)
            ctx.lineTo(leftMargin, height-bottomMargin)
            ctx.lineTo(leftMargin, topMargin)
            ctx.stroke()
            ctx.closePath()

            // Draw title
            ctx.textBaseline = "middle"
            ctx.textAlign = "center"
            ctx.font = "32px sans-serif"
            ctx.fillStyle = Qt.rgba(0, 0, 0, 1)
            ctx.fillText(title, width/2, topMargin/2)
        }
    }

    function scrollLeft(pixels) {
        if(xaxis == null || yaxis == null || yaxis.length < 1) {
            return
        }
        var xmin = xaxis.min
        var xmax = xaxis.max
        var pixels_to_graph = (xmax-xmin)/(width-leftMargin-rightMargin)
        var lastShiftX = shiftX
        shiftX -= pixels*pixels_to_graph
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
        var ymin = yaxis[0].min
        var ymax = yaxis[0].max
        var pixels_to_graph = (ymax-ymin)/(height-bottomMargin-topMargin)
        var lastShiftY = shiftY
        shiftY -= pixels*pixels_to_graph
        if(Math.floor(lastShiftY) - Math.floor(shiftY) != 0) {
            canvas.requestPaint()
        }
    }

    function scrollUp(pixels) {
        scrollDown(-1*pixels)
    }

    function zoomIn() {
        zoom /= 1.5
        canvas.requestPaint()
    }

    function zoomOut() {
        zoom *= 1.5
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
        for(var i = 0; i < series.length; i++) {
            series[i].pointAdded.connect(canvas.requestPaint)
            series[i].chart = root
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
}
