import gradientHelpers as gh
import nLevelLinearGradient as lg

if __name__ == '__main__':
    import sys
    color = []
    color[0] = '#'+sys.argv[1]
    color[1] = '#'+sys.argv[2]
    period = int(sys.argv[3],10)
    import gradientHelpers as gh
    matrix = gh.getCrossColors(color[0],color[1],period)
    columns = 60*period
    for i in range(0,columns):
        next = matrix[i][:]
        lg.led_output(next)
        time.sleep(period)
    pass
