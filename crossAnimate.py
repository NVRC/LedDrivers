import gradientHelpers as gh
import nLevelLinearGradient as lg

if __name__ == '__main__':
    import sys
    color1 = '#'+sys.argv[1]
    color2 = '#'+sys.argv[2]
    period = int(sys.argv[3],10)
    color = lg.linear_gradient(color1,color2,60)
    matrix = gh.getCrossColors(color,period)
    columns = 60*period
    for i in range(0,columns):
        next = matrix[i][:]
        lg.led_output(next)
        time.sleep(period)
