pipeline {
    agent {
        dockerfile {
              filename 'Dockerfile.build'
              args '-u 0'
        }
    }
    stages {
        stage('Build') {
            steps {
                sh 'pwd'
                sh 'cd qasm_flex_bison && mkdir build && cd build && cmake ../library && make'
            }
        }
        stage('Test') {
            steps {
               sh 'cd qasm_flex_bison && cd build && make test'
            }
        }

    }
}
