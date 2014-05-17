require './Opencl_integration'
include Opencl_integration

def rb_integrate(lower_bound, upper_bound, n, &f)
      d=(upper_bound-lower_bound) / n.to_f
      n.times.inject(0) {|ac,i| 
        ac+f[lower_bound+d*(i+0.5)]
      }*d
end

def performance_test(lower_bound, upper_bound, max_steps, file_path)
    File.open(file_path, "a") do |line|
        v = max_steps / 5000000
        for k in 1..v
            steps = k * 5000000
            start = Time.now
            integrate(lower_bound, upper_bound, steps, "x*x+2*x+1")
            finish = Time.now
            opencl = finish-start

            start = Time.now
            rb_integrate(lower_bound, upper_bound, steps){|x| x**2+2*x+1}
            finish = Time.now
            ruby = finish-start

            line.write("#{steps} #{opencl} #{ruby}\n")
        end
    end
end

performance_test(0, 4, 50000000, "/home/lahiru/performanceTest/out.txt")
