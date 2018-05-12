import gradientHelpers as gh
import nLevelLinearGradient as lg

if __name__ == '__main__':
    import sys
    color = []
    color1 = '#'+sys.argv[0]
    color2 = '#'+sys.argv[1]
    period = int(sys.argv[2],10)
    import gradientHelpers as gh
    matrix = gh.getCrossColors(color1,color2,period)
    columns = 60*period
    for i in range(0,columns):
        next = matrix[i][:]
        lg.led_output(next)
        time.sleep(period)
    pass
