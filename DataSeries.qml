import QtQuick
import QtCharts

LineSeries {
    id: altitude_series
    name: "Altitude"
    onPointAdded: function(index) {
        let x = this.at(index).x
        let y = this.at(index).y
        // create lower and upper rounded values
        let hiX = Math.ceil(x/10)*10 + 10
        let lowX = Math.floor(x/10)*10 - 10
        let hiY = Math.ceil(y/10)*10 + 10
        let lowY = Math.floor(y/10)*10 - 10
        // find new min and max
        if(x < minX) minX = lowX
        if(x > maxX) maxX = hiX
        if(y < minY) minY = lowY
        if(y > maxY) maxY = hiY

        if(!autoScale) {
            // Don't actually rescale if this is false
            return
        }

        // multiple series can be on an axis so make sure we aren't making it smaller
        // also could be in different positions
        if(this.axisX) {
            if(minX < this.axisX.min) {
                this.axisX.min = minX
            }
            if(maxX > this.axisX.max) {
                this.axisX.max = maxX
            }
        }
        if(this.axisXRight) {
            if(minX < this.axisXRight.min) {
                this.axisXRight.min = minX
            }
            if(maxX > this.axisXRight.max) {
                this.axisXRight.max = maxX
            }
        }
        if(this.axisY) {
            if(minY < this.axisY.min) {
                this.axisY.min = minY
            }
            if(maxY > this.axisY.max) {
                this.axisY.max = maxY
            }
        }
        if(this.axisYTop) {
            if(minY < this.axisYTop.min) {
                this.axisYTop.min = minY
            }
            if(maxY > this.axisYTop.max) {
                this.axisYTop.max = maxY
            }
        }
    }

    function rescaleAxes() {
        // rescale axes
        if(this.axisX) {
            this.axisX.min = minX
            this.axisX.max = maxX
        }
        if(this.axisXRight) {
            this.axisXRight.min = minX
            this.axisXRight.max = maxX
        }
        if(this.axisY) {
            this.axisY.min = minY
            this.axisY.max = maxY
        }
        if(this.axisYTop) {
            this.axisYTop.min = minY
            this.axisYTop.max = maxY
        }
    }
}
